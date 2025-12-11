// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryEquipmentManagerComponent.h"

#include "EmberPlayerState.h"
#include "ItemInstance.h"
#include "Fragments/ItemFragment_Equipable.h"
#include "InventoryManagerComponent.h"
#include "Engine/ActorChannel.h"
#include "Fragments/ItemFragment_Equipable_Armor.h"
#include "Fragments/ItemFragment_Equipable_Weapon.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "UI/Data/EmberItemData.h"

void FEquipmentEntry::Init(UItemInstance* InItemInstance, int32 InItemCount)
{
	check(InItemInstance && InItemCount > 0);

	UEquipmentManagerComponent* EquipmentManager = InventoryEquipmentManager->GetEquipmentManager();
	if (EquipmentManager == nullptr)
		return;

	const UItemFragment_Equipable* EquippableFragment = InItemInstance->FindFragmentByClass<UItemFragment_Equipable>();
	if (EquippableFragment == nullptr)
		return;

	if (ItemInstance)
	{
		EquipmentManager->Unequip(EquipmentSlotType);
	}
	
	ItemInstance = InItemInstance;

	if (InventoryEquipmentManager == nullptr)
		return;
	
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
	ItemCount = FMath::Clamp(InItemCount, 1, ItemTemplate.MaxStackCount);
	
	if ((EEquipmentSlotType::Utility_Primary <= EquipmentSlotType && EquipmentSlotType <= EEquipmentSlotType::Utility_Quaternary) == false)
		EquipmentManager->Equip(EquipmentSlotType, ItemInstance);
}

UItemInstance* FEquipmentEntry::Reset()
{
	UEquipmentManagerComponent* EquipmentManager = InventoryEquipmentManager->GetEquipmentManager();
	if (EquipmentManager == nullptr)
		return nullptr;

	if (ItemInstance)
	{
		EquipmentManager->Unequip(EquipmentSlotType);
	}
	
	UItemInstance* RemovedItemInstance = ItemInstance;
	ItemInstance = nullptr;
	ItemCount = 0;

	if (InventoryEquipmentManager->IsEquipmentSlotEmptyByEquipState(EquipmentManager->GetCurrentEquipState()))
	{
		EquipmentManager->ChangeEquipState(EEquipState::Unarmed);
	}
	
	return RemovedItemInstance;
}

bool FEquipmentList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FEquipmentEntry, FEquipmentList>(Entries, DeltaParams, *this);
}

void FEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 AddedIndex : AddedIndices)
	{
		FEquipmentEntry& Entry = Entries[AddedIndex];
		Entry.InventoryEquipmentManager = InventoryEquipmentManager;
		Entry.EquipmentSlotType = (EEquipmentSlotType)AddedIndex;

		if (Entry.GetItemInstance())
		{
			BroadcastChangedMessage((EEquipmentSlotType)AddedIndex, Entry.GetItemInstance(), Entry.GetItemCount());
		}
	}
}

void FEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 ChangedIndex : ChangedIndices)
	{
		const FEquipmentEntry& Entry = Entries[ChangedIndex];
		BroadcastChangedMessage((EEquipmentSlotType)ChangedIndex, Entry.GetItemInstance(), Entry.GetItemCount());
	}
}

void FEquipmentList::CustomMarkItemDirty(AActor* Owner, FEquipmentEntry& Entry)
{
	MarkItemDirty(Entry);
	
	if (Owner && Owner->IsNetMode(NM_ListenServer))
	{
		BroadcastChangedMessage(Entry.EquipmentSlotType, Entry.GetItemInstance(), Entry.GetItemCount());
	}
}

void FEquipmentList::BroadcastChangedMessage(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance, int32 ItemCount)
{
	if (InventoryEquipmentManager->OnEquipmentEntryChanged.IsBound())
	{
		InventoryEquipmentManager->OnEquipmentEntryChanged.Broadcast(EquipmentSlotType, ItemInstance, ItemCount);
	}
}


UInventoryEquipmentManagerComponent::UInventoryEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;

	CurrentEquipState = EEquipState::Weapon_Primary;
}

void UInventoryEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		TArray<FEquipmentEntry>& Entries = EquipmentList.Entries;
		Entries.SetNum((int32)EEquipmentSlotType::Count);

		for (int32 i = 0; i < Entries.Num(); i++)
		{
			FEquipmentEntry& Entry = Entries[i];
			Entry.InventoryEquipmentManager = this;
			Entry.EquipmentSlotType = (EEquipmentSlotType)i;
			EquipmentList.MarkItemDirty(Entry);
		}
	}
}

void UInventoryEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

bool UInventoryEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	for (FEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UItemInstance* ItemInstance = Entry.ItemInstance;
		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}
	
	return bWroteSomething;
}

void UInventoryEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const FEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UItemInstance* ItemInstance = Entry.GetItemInstance();
			if (IsValid(ItemInstance))
			{
				AddReplicatedSubObject(ItemInstance);
			}
		}
	}
}

void UInventoryEquipmentManagerComponent::AddEquipment_Unsafe(EEquipmentSlotType EquipmentSlotType, UItemInstance* FromItemInstance, int32 FromItemCount)
{
	check(HasAuthority());

	if (EquipmentSlotType == EEquipmentSlotType::Count || FromItemCount <= 0)
		return;

	FEquipmentEntry& Entry = EquipmentList.Entries[(int32)EquipmentSlotType];
	if (Entry.GetItemInstance())
	{
		Entry.ItemCount += FromItemCount;
	}
	else
	{
		if (FromItemInstance == nullptr)
			return;
		
		Entry.Init(FromItemInstance, FromItemCount);

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && FromItemInstance)
		{
			AddReplicatedSubObject(FromItemInstance);
		}
	}
	
	EquipmentList.CustomMarkItemDirty(GetOwner(), Entry);
}

UItemInstance* UInventoryEquipmentManagerComponent::RemoveEquipment_Unsafe(EEquipmentSlotType EquipmentSlotType, int32 ItemCount)
{
	check(HasAuthority());

	FEquipmentEntry& Entry = EquipmentList.Entries[(int32)EquipmentSlotType];
	UItemInstance* ItemInstance = Entry.GetItemInstance();

	Entry.ItemCount -= ItemCount;
	if (Entry.GetItemCount() <= 0)
	{
		UItemInstance* RemovedItemInstance = Entry.Reset();
		if (IsUsingRegisteredSubObjectList() && RemovedItemInstance)
		{
			RemoveReplicatedSubObject(RemovedItemInstance);
		}
	}

	EquipmentList.CustomMarkItemDirty(GetOwner(), Entry);
	return ItemInstance;
}

void UInventoryEquipmentManagerComponent::AddUnarmedEquipments(TSubclassOf<UItemTemplate> RightHandClass)
{
	check(HasAuthority());

	SetEquipment(EEquipmentSlotType::Unarmed_RightHand, RightHandClass, EItemRarity::Common, 1, false);
}

void UInventoryEquipmentManagerComponent::SetEquipment(EEquipmentSlotType EquipmentSlotType, TSubclassOf<UItemTemplate> ItemTemplateClass, EItemRarity ItemRarity, int32 ItemCount, bool bCheckCharacterClass)
{
	check(HasAuthority());
	
	if (EquipmentSlotType == EEquipmentSlotType::Count || ItemTemplateClass == nullptr || ItemRarity == EItemRarity::Count || ItemCount <= 0)
		return;
	
	const int32 ItemTemplateID = UEmberItemData::Get().FindItemTemplateIDByClass(ItemTemplateClass);
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);

	ItemCount = FMath::Clamp(ItemCount, 1, ItemTemplate.MaxStackCount);
	
	const UItemFragment_Equipable* EquippableFragment = ItemTemplate.FindFragmentByClass<UItemFragment_Equipable>();
	if (EquippableFragment == nullptr)
		return;

	if (bCheckCharacterClass)
	{
		AEmberPlayerState* LyraPlayerState = GetPlayerState<AEmberPlayerState>();
		if (LyraPlayerState == nullptr)
			return;
	}
	
	FEquipmentEntry& Entry = EquipmentList.Entries[(int32)EquipmentSlotType];
	Entry.Reset();

	UItemInstance* AddedItemInstance = NewObject<UItemInstance>();
	AddedItemInstance->Init(ItemTemplateID, ItemRarity);
	Entry.Init(AddedItemInstance, ItemCount);

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && AddedItemInstance)
	{
		AddReplicatedSubObject(AddedItemInstance);
	}
	
	EquipmentList.MarkItemDirty(Entry);
	
	if (UEquipmentManagerComponent* EquipManager = GetEquipmentManager())
	{
		EEquipState EquipState = EEquipState::Weapon_Primary;
		if (EquipManager->GetCurrentEquipState() != EquipState)
		{
			EquipManager->ChangeEquipState(EquipState);
		}
	}
}

int32 UInventoryEquipmentManagerComponent::CanMoveOrMergeEquipment(UInventoryEquipmentManagerComponent* OtherComponent, EEquipmentSlotType FromEquipmentSlotType, EEquipmentSlotType ToEquipmentSlotType) const
{
	if (OtherComponent == nullptr || FromEquipmentSlotType == EEquipmentSlotType::Count)
		return 0;

	const UItemInstance* FromItemInstance = OtherComponent->GetItemInstance(FromEquipmentSlotType);
	if (FromItemInstance == nullptr)
		return 0;
	
	const int32 FromItemCount = OtherComponent->GetItemCount(FromEquipmentSlotType);
	if (IsSameComponent(OtherComponent) && FromEquipmentSlotType == ToEquipmentSlotType)
		return FromItemCount;

	return CanAddEquipment(FromItemInstance->GetItemTemplateID(), FromItemInstance->GetItemRarity(), FromItemCount, ToEquipmentSlotType);
}

int32 UInventoryEquipmentManagerComponent::CanMoveOrMergeEquipment(UInventoryManagerComponent* OtherComponent, const FIntPoint& FromItemSlotPos, EEquipmentSlotType ToEquipmentSlotType) const
{
	if (OtherComponent == nullptr)
		return 0;

	const FIntPoint& FromInventorySlotCount = OtherComponent->GetInventorySlotCount();
	if (FromItemSlotPos.X < 0 || FromItemSlotPos.Y < 0 || FromItemSlotPos.X >= FromInventorySlotCount.X || FromItemSlotPos.Y >= FromInventorySlotCount.Y)
		return 0;

	const UItemInstance* FromItemInstance = OtherComponent->GetItemInstance(FromItemSlotPos);
	if (FromItemInstance == nullptr)
		return 0;
	
	const int32 FromItemCount = OtherComponent->GetItemCount(FromItemSlotPos);
	return CanAddEquipment(FromItemInstance->GetItemTemplateID(), FromItemInstance->GetItemRarity(), FromItemCount, ToEquipmentSlotType);
}

bool UInventoryEquipmentManagerComponent::CanSwapEquipment(UInventoryEquipmentManagerComponent* OtherComponent, EEquipmentSlotType FromEquipmentSlotType, EEquipmentSlotType ToEquipmentSlotType) const
{
	if (IsSameComponent(OtherComponent) && FromEquipmentSlotType == ToEquipmentSlotType)
		return false;
	
	UItemInstance* FromItemInstance = GetItemInstance(OtherComponent, FromEquipmentSlotType);
	if (FromItemInstance == nullptr)
		return false;
	
	const UItemInstance* ToItemInstance = GetItemInstance(ToEquipmentSlotType);
	if (ToItemInstance == nullptr)
		return false;

	const UItemFragment_Equipable* FromEquippableFragment = FromItemInstance->FindFragmentByClass<UItemFragment_Equipable>();
	if (FromEquippableFragment == nullptr)
		return false;

	if (FromEquippableFragment->EquipmentType == EEquipmentType::Count)
		return false;

	return true;
}

bool UInventoryEquipmentManagerComponent::CanSwapEquipment(UInventoryManagerComponent* OtherComponent, const FIntPoint& FromItemSlotPos, EEquipmentSlotType ToEquipmentSlotType, FIntPoint& OutToItemSlotPos)
{
	if (OtherComponent == nullptr || ToEquipmentSlotType == EEquipmentSlotType::Count)
		return false;

	const FIntPoint& FromInventorySlotCount = OtherComponent->GetInventorySlotCount();
	if (FromItemSlotPos.X < 0 || FromItemSlotPos.Y < 0 || FromItemSlotPos.X >= FromInventorySlotCount.X || FromItemSlotPos.Y >= FromInventorySlotCount.Y)
		return false;

	UItemInstance* FromItemInstance = OtherComponent->GetItemInstance(FromItemSlotPos);
	if (FromItemInstance == nullptr)
		return false;

	const UItemInstance* ToItemInstance = GetItemInstance(ToEquipmentSlotType);
	if (ToItemInstance == nullptr)
		return false;

	const UItemFragment_Equipable* FromEquippableFragment = FromItemInstance->FindFragmentByClass<UItemFragment_Equipable>();
	if (FromEquippableFragment == nullptr)
		return false;

	const EEquipmentType FromEquipmentType = FromEquippableFragment->EquipmentType;
	if (IsMatchingSlotType(FromEquipmentType, ToEquipmentSlotType) == false)
		return false;

	const UItemTemplate& FromItemTemplate = UEmberItemData::Get().FindItemTemplateByID(FromItemInstance->GetItemTemplateID());
	const FIntPoint& FromSlotCount = FromItemTemplate.SlotCount;
			
	const UItemTemplate& ToItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ToItemInstance->GetItemTemplateID());
	const FIntPoint& ToSlotCount = ToItemTemplate.SlotCount;

	TArray<bool> TempSlotChecks = OtherComponent->SlotChecks;
	OtherComponent->MarkSlotChecks(TempSlotChecks, false, FromItemSlotPos, FromSlotCount);

	if (OtherComponent->IsEmpty(TempSlotChecks, FromItemSlotPos, ToSlotCount))
	{
		OutToItemSlotPos = FromItemSlotPos;
		return true;
	}
	else
	{
		const FIntPoint StartSlotPos = FIntPoint::ZeroValue;
		const FIntPoint EndSlotPos = FromInventorySlotCount - ToSlotCount;
	
		for (int32 y = StartSlotPos.Y; y <= EndSlotPos.Y; y++)
		{
			for (int32 x = StartSlotPos.X; x <= EndSlotPos.X; x++)
			{
				int32 Index = y * FromInventorySlotCount.X + x;
				if (TempSlotChecks.IsValidIndex(Index) == false || TempSlotChecks[Index])
					continue;

				FIntPoint ToItemSlotPos = FIntPoint(x, y);
				if (OtherComponent->IsEmpty(TempSlotChecks, ToItemSlotPos, ToSlotCount))
				{
					OutToItemSlotPos = ToItemSlotPos;
					return true;
				}
			}
		}
	}

	return false;
}

int32 UInventoryEquipmentManagerComponent::CanAddEquipment(int32 FromItemTemplateID, EItemRarity FromItemRarity, int32 FromItemCount, EEquipmentSlotType ToEquipmentSlotType) const
{
	if (FromItemTemplateID <= 0 || FromItemRarity == EItemRarity::Count || FromItemCount <= 0)
		return 0;

	if (ToEquipmentSlotType == EEquipmentSlotType::Count)
		return 0;

	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(FromItemTemplateID);
	const UItemFragment_Equipable* FromEquippableFragment = ItemTemplate.FindFragmentByClass<UItemFragment_Equipable>();
	if (FromEquippableFragment == nullptr)
		return 0;

	const UItemInstance* ToItemInstance = GetItemInstance(ToEquipmentSlotType);
	const int32 ToItemCount = GetItemCount(ToEquipmentSlotType);

	if (ToItemInstance)
	{
		if (FromEquippableFragment->EquipmentType == EEquipmentType::Weapon || FromEquippableFragment->EquipmentType == EEquipmentType::Armor)
			return 0;
		
		const int32 ToTemplateID = ToItemInstance->GetItemTemplateID();
		if (FromItemTemplateID != ToTemplateID)
			return 0;

		if (FromItemRarity != ToItemInstance->GetItemRarity())
			return 0;
		
		if (ItemTemplate.MaxStackCount <= 1)
			return 0;

		return FMath::Min(FromItemCount + ToItemCount, ItemTemplate.MaxStackCount) - ToItemCount;
	}
	else
	{
		AEmberPlayerState* EmberPlayerState = GetPlayerState<AEmberPlayerState>();
		if (EmberPlayerState == nullptr)
			return 0;
		
		if (IsMatchingSlotType(FromEquippableFragment, ToEquipmentSlotType) == false)
			return 0;

		// TODO: 캐릭터 능력에 따른 아이템 장착 제한 구현
	}

	return FromItemCount;
}

bool UInventoryEquipmentManagerComponent::IsMatchingSlotType(EEquipmentType FromEquipmentType, EEquipmentSlotType ToEquipmentSlotType) const
{
	if (FromEquipmentType == EEquipmentType::Weapon)
	{
		return ToEquipmentSlotType >= EEquipmentSlotType::Primary_LeftHand && ToEquipmentSlotType <= EEquipmentSlotType::Primary_TwoHand;
	}
	if (FromEquipmentType == EEquipmentType::Armor)
	{
		return ToEquipmentSlotType >= EEquipmentSlotType::Helmet && ToEquipmentSlotType <= EEquipmentSlotType::Foot;
	}
	if (FromEquipmentType == EEquipmentType::Utility)
	{
		return ToEquipmentSlotType >= EEquipmentSlotType::Utility_Primary && ToEquipmentSlotType <= EEquipmentSlotType::Utility_Quaternary;
	}

	return false;
}

bool UInventoryEquipmentManagerComponent::IsMatchingSlotType(const UItemFragment_Equipable* FromEquippableFragment, EEquipmentSlotType ToEquipmentSlotType) const
{
	if (FromEquippableFragment == nullptr)
		return false;
	
	if (FromEquippableFragment->EquipmentType == EEquipmentType::Weapon)
	{
		if (ToEquipmentSlotType != EEquipmentSlotType::Primary_LeftHand && ToEquipmentSlotType != EEquipmentSlotType::Primary_RightHand)
		{
			return false;
		}
		
		const UItemFragment_Equipable_Weapon* FromWeaponFragment = Cast<UItemFragment_Equipable_Weapon>(FromEquippableFragment);
		EWeaponHandType FromWeaponHandType = FromWeaponFragment->WeaponHandType;

		if (IsSameWeaponHandType(ToEquipmentSlotType, FromWeaponHandType) == false)
		{
			return false;
		}

		if (FromWeaponHandType == EWeaponHandType::TwoHand)
		{
			return GetItemInstance(EEquipmentSlotType::Primary_LeftHand) == nullptr
				|| GetItemInstance(EEquipmentSlotType::Primary_RightHand) == nullptr;
		}

		return true;
	}
	else if (FromEquippableFragment->EquipmentType == EEquipmentType::Armor)
	{
		const UItemFragment_Equipable_Armor* FromArmorFragment = Cast<UItemFragment_Equipable_Armor>(FromEquippableFragment);
		const EArmorType ArmorType = FromArmorFragment->ArmorType;
		
		return (ToEquipmentSlotType == EEquipmentSlotType::Helmet	&& ArmorType == EArmorType::Helmet)
			|| (ToEquipmentSlotType == EEquipmentSlotType::Shoulder && ArmorType == EArmorType::Shoulder)
			|| (ToEquipmentSlotType == EEquipmentSlotType::Chest	&& ArmorType == EArmorType::Chest)
			|| (ToEquipmentSlotType == EEquipmentSlotType::Cloak	&& ArmorType == EArmorType::Cloak)
			|| (ToEquipmentSlotType == EEquipmentSlotType::Gloves   && ArmorType == EArmorType::Gloves)
			|| (ToEquipmentSlotType == EEquipmentSlotType::Pants	&& ArmorType == EArmorType::Pants)
			|| (ToEquipmentSlotType == EEquipmentSlotType::Foot		&& ArmorType == EArmorType::Foot);
	}
	else if (FromEquippableFragment->EquipmentType == EEquipmentType::Utility)
	{
		return (EEquipmentSlotType::Utility_Primary <= ToEquipmentSlotType && ToEquipmentSlotType <= EEquipmentSlotType::Utility_Quaternary);
	}

	return false;
}

bool UInventoryEquipmentManagerComponent::IsSlotEquipped(EEquipmentSlotType EquipmentSlotType) const
{
	if (EquipmentSlotType == EEquipmentSlotType::Count)
		return false;

	const TArray<FEquipmentEntry>& Entries = GetAllEntries();
	const FEquipmentEntry& Entry = Entries[(int32)EquipmentSlotType];
	
	return Entry.GetItemInstance() != nullptr;
}

bool UInventoryEquipmentManagerComponent::IsSameEquipState(EEquipmentSlotType EquipmentSlotType, EEquipState WeaponEquipState) const
{
	if (WeaponEquipState == EEquipState::Unarmed)
		return EquipmentSlotType == EEquipmentSlotType::Unarmed_LeftHand || EquipmentSlotType == EEquipmentSlotType::Unarmed_RightHand;
	if (WeaponEquipState == EEquipState::Weapon_Primary)
		return EquipmentSlotType == EEquipmentSlotType::Primary_LeftHand || EquipmentSlotType == EEquipmentSlotType::Primary_RightHand || EquipmentSlotType == EEquipmentSlotType::Primary_TwoHand;
	if (WeaponEquipState == EEquipState::Utility_Primary)
		return EquipmentSlotType == EEquipmentSlotType::Utility_Primary;
	if (WeaponEquipState == EEquipState::Utility_Secondary)
		return EquipmentSlotType == EEquipmentSlotType::Utility_Secondary;
	if (WeaponEquipState == EEquipState::Utility_Tertiary)
		return EquipmentSlotType == EEquipmentSlotType::Utility_Tertiary;
	if (WeaponEquipState == EEquipState::Utility_Quaternary)
		return EquipmentSlotType == EEquipmentSlotType::Utility_Quaternary;

	return false;
}

bool UInventoryEquipmentManagerComponent::IsSameWeaponHandType(EEquipmentSlotType EquipmentSlotType, EWeaponHandType WeaponHandType) const
{
	if (WeaponHandType == EWeaponHandType::LeftHand)
		return EquipmentSlotType == EEquipmentSlotType::Primary_LeftHand;
	if (WeaponHandType == EWeaponHandType::RightHand)
		return EquipmentSlotType == EEquipmentSlotType::Primary_RightHand;
	if (WeaponHandType == EWeaponHandType::TwoHand)
		return EquipmentSlotType == EEquipmentSlotType::Primary_TwoHand;

	return false;
}

bool UInventoryEquipmentManagerComponent::IsSameComponent(const UInventoryEquipmentManagerComponent* OtherComponent) const
{
	return this == OtherComponent;
}

bool UInventoryEquipmentManagerComponent::IsEquipmentSlotEmptyByEquipState(EEquipState EquipState)
{
	if (EquipState == EEquipState::Count)
		return true;

	if (EquipState == EEquipState::Unarmed)
		return false;

	bool bAllEmpty = true;
	for (EEquipmentSlotType SlotType : UEquipmentManagerComponent::GetEquipmentSlotsByEquipState(EquipState))
	{
		const FEquipmentEntry& Entry = EquipmentList.Entries[(int32)SlotType];
		if (Entry.ItemInstance)
		{
			bAllEmpty = false;
			break;
		}
	}

	return bAllEmpty;
}

EUtilitySlotType UInventoryEquipmentManagerComponent::ConvertToUtilitySlotType(EEquipmentSlotType EquipmentSlotType)
{
	EUtilitySlotType UtilitySlotType = EUtilitySlotType::Count;

	switch (EquipmentSlotType)
	{
	case EEquipmentSlotType::Utility_Primary:		UtilitySlotType = EUtilitySlotType::Primary;	break;
	case EEquipmentSlotType::Utility_Secondary:		UtilitySlotType = EUtilitySlotType::Secondary;	break;
	case EEquipmentSlotType::Utility_Tertiary:		UtilitySlotType = EUtilitySlotType::Tertiary;	break;
	case EEquipmentSlotType::Utility_Quaternary:	UtilitySlotType = EUtilitySlotType::Quaternary;	break;
	}

	return UtilitySlotType;
}

UEquipmentManagerComponent* UInventoryEquipmentManagerComponent::GetEquipmentManager() const
{
	UEquipmentManagerComponent* EquipmentManager = nullptr;
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		EquipmentManager = Character->FindComponentByClass<UEquipmentManagerComponent>();
	}
	
	return EquipmentManager;
}

UItemInstance* UInventoryEquipmentManagerComponent::GetItemInstance(EEquipmentSlotType EquipmentSlotType) const
{
	return GetItemInstance(this, EquipmentSlotType);
}

UItemInstance* UInventoryEquipmentManagerComponent::GetItemInstance(const UInventoryEquipmentManagerComponent* OtherComponent, EEquipmentSlotType EquipmentSlotType) const
{
	if (OtherComponent == nullptr || EquipmentSlotType == EEquipmentSlotType::Count)
		return nullptr;

	const TArray<FEquipmentEntry>& Entries = OtherComponent->GetAllEntries();
	const FEquipmentEntry& Entry = Entries[(int32)EquipmentSlotType];
	
	return Entry.GetItemInstance();
}

const TArray<FEquipmentEntry>& UInventoryEquipmentManagerComponent::GetAllEntries() const
{
	return EquipmentList.GetAllEntries();
}

int32 UInventoryEquipmentManagerComponent::GetItemCount(EEquipmentSlotType EquipmentSlotType) const
{
	if (EquipmentSlotType == EEquipmentSlotType::Count)
		return 0;

	const TArray<FEquipmentEntry>& Entries = GetAllEntries();
	const FEquipmentEntry& Entry = Entries[(int32)EquipmentSlotType];
	
	return Entry.GetItemCount();
}