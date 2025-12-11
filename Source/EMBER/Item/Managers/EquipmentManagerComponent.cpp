// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentManagerComponent.h"

#include "ArmorComponent.h"
#include "C_StateComponent.h"
#include "EmberPlayerCharacter.h"
#include "GameFlag.h"
#include "InventoryEquipmentManagerComponent.h"
#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "Equipment/EquipmentBase.h"
#include "Fragments/ItemFragment_Equipable_Weapon.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "UI/Data/EmberItemData.h"
#include "MontageSystemComponent.h"
#include "Engine/ActorChannel.h"
#include "Fragments/ItemFragment_Equipable_Armor.h"
#include "System/AbilitySystem/EmberAbilitySystemComponent.h"


void FEquipEntry::Init(UItemInstance* InItemInstance)
{
	if (ItemInstance == InItemInstance)
		return;

	ItemInstance = InItemInstance;
	ItemInstance ? Equip() : Unequip();
}

void FEquipEntry::Equip()
{
	if (ItemInstance == nullptr)
		return;

	if (EquipmentManager == nullptr)
		return;

	const UItemFragment_Equipable_Attachment* AttachmentFragment = ItemInstance->FindFragmentByClass<UItemFragment_Equipable_Attachment>();
	if (AttachmentFragment == nullptr)
		return;
	
 	AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(EquipmentManager->GetOwner());
	if (EmberCharacter == nullptr)
		return;
	
	if (EmberCharacter->HasAuthority())
	{
		UEmberAbilitySystemComponent* EmberASC =  Cast<UEmberAbilitySystemComponent>(EmberCharacter->GetAbilitySystemComponent());
		if (EmberASC == nullptr)
			return;
		
		// 이전 아이템 Ability 삭제
		BaseAbilitySetHandles.TakeFromAbilitySystem(EmberASC);
		
		// 현재 아이템 Ability 적용
		if (const UEmberAbilitySet* BaseAbilitySet = AttachmentFragment->BaseAbilitySet)
		{
			BaseAbilitySet->GiveToAbilitySystem(EmberASC, &BaseAbilitySetHandles,  const_cast<UItemFragment_Equipable_Attachment*>(AttachmentFragment));
		}
	}
	
	// 손에 장착할 아이템 생성
	if (AttachmentFragment->EquipmentType == EEquipmentType::Weapon || AttachmentFragment->EquipmentType == EEquipmentType::Utility)
	{
		if (IsValid(SpawnedEquipmentActor))
		{
			SpawnedEquipmentActor->Destroy();
		}
		
		const FItemAttachInfo& AttachInfo = AttachmentFragment->ItemAttachInfo;
		if (AttachInfo.SpawnEquipmentClass)
		{
			UWorld* World = EquipmentManager->GetWorld();
			AEquipmentBase* NewWeaponActor = World->SpawnActorDeferred<AEquipmentBase>(AttachInfo.SpawnEquipmentClass,FTransform::Identity, EmberCharacter);
			NewWeaponActor->Init(ItemInstance->GetItemTemplateID(), EquipmentSlotType);
			NewWeaponActor->SetActorRelativeTransform(AttachInfo.AttachTransform);
			NewWeaponActor->AttachToComponent(EmberCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachInfo.AttachSocket);
			NewWeaponActor->FinishSpawning(FTransform::Identity, true);
		}
	}
}

void FEquipEntry::Unequip()
{
	if (EquipmentManager == nullptr)
		return;

	AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(EquipmentManager->GetOwner());
	if (EmberCharacter == nullptr)
		return;

	UEmberAbilitySystemComponent* EmberASC =  Cast<UEmberAbilitySystemComponent>(EmberCharacter->GetAbilitySystemComponent());
	if (EmberASC == nullptr)
		return;
	
	if (EmberCharacter->HasAuthority())
	{
		// 이전 아이템 Ability 삭제
		BaseAbilitySetHandles.TakeFromAbilitySystem(EmberASC);

		EmberASC->RemoveActiveGameplayEffect(BaseStatHandle);
		BaseStatHandle.Invalidate();
		
		if (IsValid(SpawnedEquipmentActor))
		{
			SpawnedEquipmentActor->Destroy();
		}
	}
}

bool FEquipList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FEquipEntry, FEquipList>(Entries, DeltaParms,*this);

}

void FEquipList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 AddedIndex : AddedIndices)
	{
		FEquipEntry& Entry = Entries[AddedIndex];
		Entry.EquipmentManager = EquipmentManager;
		Entry.EquipmentSlotType = (EEquipmentSlotType)AddedIndex;
		
		if (Entry.GetItemInstance())
		{
			Entry.Equip();
		}
	}
}

void FEquipList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 ChangedIndex : ChangedIndices)
	{
		FEquipEntry& Entry = Entries[ChangedIndex];
		Entry.GetItemInstance() ? Entry.Equip() : Entry.Unequip();
	}
}

void FEquipList::Equip(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance)
{
	FEquipEntry& Entry = Entries[(int32)(EquipmentSlotType)];
	Entry.Init(ItemInstance);
	MarkItemDirty(Entry);
}

void FEquipList::Unequip(EEquipmentSlotType EquipmentSlotType)
{
	FEquipEntry& Entry = Entries[(int32)(EquipmentSlotType)];
	Entry.Init(nullptr);
	MarkItemDirty(Entry);
}

void FEquipList::Equip_UnSafe(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance)
{
	FEquipEntry& Entry = Entries[(int32)(EquipmentSlotType)];
	Entry.ItemInstance = ItemInstance;
	MarkItemDirty(Entry);
}

void FEquipList::Unequip_UnSafe(EEquipmentSlotType EquipmentSlotType)
{
	FEquipEntry& Entry = Entries[(int32)(EquipmentSlotType)];
	Entry.ItemInstance = nullptr;
	MarkItemDirty(Entry);
}

UEquipmentManagerComponent::UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), EquipList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Owner Characte is null");
		return;
	}
	ArmorComponent = OwnerCharacter->FindComponentByClass<UArmorComponent>();
	if (ArmorComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Armor Component is null");
		return;
	}
	MontageComponent = OwnerCharacter->FindComponentByClass<UMontageSystemComponent>();
	if (MontageComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Montage Component is null");
		return;
	}
	StateComponent = OwnerCharacter->FindComponentByClass<UC_StateComponent>();
	if (StateComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, L"State Component is null");
		return;
	}
}

void UEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		TArray<FEquipEntry>& Entries = EquipList.Entries;
		Entries.SetNum((int32)EEquipmentSlotType::Count);

		for (int32 i = 0; i < Entries.Num(); i++)
		{
			FEquipEntry& Entry = Entries[i];
			Entry.EquipmentManager = this;
			Entry.EquipmentSlotType = (EEquipmentSlotType)i;
			EquipList.MarkItemDirty(Entry);
		}
	}

	CurrentEquipState = EEquipState::Weapon_Primary;
}

void UEquipmentManagerComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		for (int32 i = 0; i < (int32)EEquipmentSlotType::Count; i++)
		{
			Unequip((EEquipmentSlotType)i);
		}
	}
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemTemplateID);
	DOREPLIFETIME(ThisClass, EquipList);
}

bool UEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	for (FEquipEntry& Entry : EquipList.Entries)
	{
		UItemInstance* ItemInstance = Entry.GetItemInstance();
		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}
	
	return bWroteSomething;
}

void UEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const FEquipEntry& Entry : EquipList.Entries)
		{
			UItemInstance* ItemInstance = Entry.GetItemInstance();
			if (IsValid(ItemInstance))
			{
				AddReplicatedSubObject(ItemInstance);
			}
		}
	}
}

void UEquipmentManagerComponent::AddEquipment(EWeaponSlotType WeaponSlotType, TSubclassOf<UItemTemplate> ItemTemplateClass)
{
	if (WeaponSlotType == EWeaponSlotType::Count || ItemTemplateClass == nullptr)
		return;

	const UItemTemplate* ItemTemplate = ItemTemplateClass.GetDefaultObject();
	if (ItemTemplate == nullptr)
		return;
	
	if (OwnerCharacter == nullptr)
		return;

	USkeletalMeshComponent* CharacterMeshComponent = OwnerCharacter->GetMesh();
	if (CharacterMeshComponent == nullptr)
		return;

	UItemInstance* ItemInstance = NewObject<UItemInstance>();
	int32 NewItemTemplateID = UEmberItemData::Get().FindItemTemplateIDByClass(ItemTemplateClass);

	EItemRarity RandomItemRarity = (EItemRarity)FMath::RandRange(0, (int32)EItemRarity::Count - 1);
	ItemInstance->Init(NewItemTemplateID, RandomItemRarity);
	
	if (const UItemFragment_Equipable_Attachment* AttachmentFragment = ItemInstance->FindFragmentByClass
		<UItemFragment_Equipable_Attachment> ())
	{
		if (SpawnedWeapon != nullptr)
		{
			SpawnedWeapon->Destroy();
			if (ItemTemplateID == NewItemTemplateID)
			{
				ItemTemplateID = INDEX_NONE;
				return;
			}
		}
		// 아이템 스폰
		const FItemAttachInfo& AttachInfo = AttachmentFragment->ItemAttachInfo;
		if (AttachInfo.SpawnEquipmentClass)
		{
			UWorld* World = GetWorld();
			SpawnedWeapon = World->SpawnActorDeferred<AEquipmentBase>(AttachInfo.SpawnEquipmentClass,
			                                                          FTransform::Identity, OwnerCharacter);
			SpawnedWeapon->Init(NewItemTemplateID, EEquipmentSlotType::Primary_LeftHand);
			SpawnedWeapon->SetActorRelativeTransform(AttachInfo.AttachTransform);
			SpawnedWeapon->AttachToComponent(CharacterMeshComponent, FAttachmentTransformRules::KeepRelativeTransform,
			                                 AttachInfo.AttachSocket);
			SpawnedWeapon->FinishSpawning(FTransform::Identity, true);

			FEquipment Data = AttachmentFragment->GetEquipmentInfo();

			if (Data.DrawMontage)
			{
				if (UMontageSystemComponent* MontageComp = OwnerCharacter->FindComponentByClass
					<UMontageSystemComponent > ()
				)
				{
					MontageComp->PlayMontage(Data.DrawMontage);
				}
			}
		}

		ItemTemplateID = NewItemTemplateID;
	}
	else
	{
		ArmorComponent.Get()->DetermineEquip(ItemTemplate->FindFragmentByClass<UItemFragment_Equipable_Armor>(), NewItemTemplateID);
	}
}

void UEquipmentManagerComponent::Equip(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance)
{
	check(GetOwner()->HasAuthority());

	if (EquipmentSlotType == EEquipmentSlotType::Count || ItemInstance == nullptr)
		return;

	const UItemFragment_Equipable* EquippableFragment = ItemInstance->FindFragmentByClass<UItemFragment_Equipable>();
	if (EquippableFragment == nullptr)
		return;
	
	
	if (EquippableFragment->EquipmentType == EEquipmentType::Weapon || EquippableFragment->EquipmentType == EEquipmentType::Utility)
	{
		EquipList.Equip(EquipmentSlotType, ItemInstance);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
		{
			AddReplicatedSubObject(ItemInstance);
		}
	}
	else if (EquippableFragment->EquipmentType == EEquipmentType::Armor)
	{
		EquipList.Equip_UnSafe(EquipmentSlotType, ItemInstance);
		Equip_Armor(ItemInstance);
	}
}

void UEquipmentManagerComponent::Unequip(EEquipmentSlotType EquipmentSlotType)
{
	check(GetOwner()->HasAuthority());

	if (EquipmentSlotType == EEquipmentSlotType::Count)
		return;

	TArray<FEquipEntry>& Entries = EquipList.Entries;
	FEquipEntry& Entry = Entries[(int32)EquipmentSlotType];
	UItemInstance* RemovedItemInstance = Entry.GetItemInstance();

	if (RemovedItemInstance == nullptr)
		return;

	const UItemFragment_Equipable* EquipableFragment = RemovedItemInstance->FindFragmentByClass<UItemFragment_Equipable>();
	if (EquipableFragment == nullptr)
		return;
	
	if (EquipableFragment->EquipmentType == EEquipmentType::Weapon || EquipableFragment->EquipmentType == EEquipmentType::Utility)
	{
		EquipList.Unequip(EquipmentSlotType);
		if (IsUsingRegisteredSubObjectList() && RemovedItemInstance)
		{
			RemoveReplicatedSubObject(RemovedItemInstance);
		}
	}
	else if (EquipableFragment->EquipmentType == EEquipmentType::Armor)
	{
		EquipList.Unequip(EquipmentSlotType);
		Unequip_Armor(RemovedItemInstance);
	}
}

AEquipmentBase* UEquipmentManagerComponent::GetHandEquipment() const
{
	// TODO : 향후 수정 코드
	return EquipList.Entries[(int32)EEquipmentSlotType::Primary_RightHand].GetEquipmentActor();
}

AEquipmentBase* UEquipmentManagerComponent::GetEquippedActor(EWeaponHandType WeaponHandType) const
{
	if (WeaponHandType == EWeaponHandType::Count)
		return nullptr;
	
	const TArray<FEquipEntry>& Entries = EquipList.Entries;
	const int32 EntryIndex = (int32)ConvertToEquipmentSlotType(WeaponHandType, CurrentEquipState);
	return Entries.IsValidIndex(EntryIndex) ? Entries[EntryIndex].GetEquipmentActor() : nullptr;
}

void UEquipmentManagerComponent::Equip_Armor(UItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
		return;
	
	int32 NewItemTemplateID = ItemInstance->GetItemTemplateID();
	ArmorComponent.Get()->DetermineEquip(ItemInstance->FindFragmentByClass<UItemFragment_Equipable_Armor>(), NewItemTemplateID);
}

void UEquipmentManagerComponent::Unequip_Armor(UItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
		return;
	
	int32 NewItemTemplateID = ItemInstance->GetItemTemplateID();
	ArmorComponent.Get()->DetermineEquip(ItemInstance->FindFragmentByClass<UItemFragment_Equipable_Armor>(), NewItemTemplateID);
}

EEquipState UEquipmentManagerComponent::ConvertToEquipState(EUtilitySlotType UtilitySlotType)
{
	EEquipState EquipState = EEquipState::Count;

	switch (UtilitySlotType)
	{
	case EUtilitySlotType::Primary:		EquipState = EEquipState::Utility_Primary;		break;
	case EUtilitySlotType::Secondary:	EquipState = EEquipState::Utility_Secondary;	break;
	case EUtilitySlotType::Tertiary:	EquipState = EEquipState::Utility_Tertiary;		break;
	case EUtilitySlotType::Quaternary:	EquipState = EEquipState::Utility_Quaternary;	break;
	}

	return EquipState;
}

EEquipmentSlotType UEquipmentManagerComponent::ConvertToEquipmentSlotType(EWeaponHandType WeaponHandType, EEquipState EquipState)
{
	EEquipmentSlotType EquipmentSlotType = EEquipmentSlotType::Count;

	if (EquipState == EEquipState::Unarmed)
	{
		switch (WeaponHandType)
		{
		case EWeaponHandType::LeftHand:  EquipmentSlotType = EEquipmentSlotType::Unarmed_LeftHand;  break;
		case EWeaponHandType::RightHand: EquipmentSlotType = EEquipmentSlotType::Unarmed_RightHand; break;
		}
	}
	else if (EquipState == EEquipState::Weapon_Primary)
	{
		switch (WeaponHandType)
		{
		case EWeaponHandType::LeftHand:  EquipmentSlotType = EEquipmentSlotType::Primary_LeftHand;  break;
		case EWeaponHandType::RightHand: EquipmentSlotType = EEquipmentSlotType::Primary_RightHand; break;
		case EWeaponHandType::TwoHand:   EquipmentSlotType = EEquipmentSlotType::Primary_TwoHand;   break;
		}
	}
	else if (EquipState == EEquipState::Utility_Primary)
	{
		EquipmentSlotType = EEquipmentSlotType::Utility_Primary;
	}
	else if (EquipState == EEquipState::Utility_Secondary)
	{
		EquipmentSlotType = EEquipmentSlotType::Utility_Secondary;
	}
	else if (EquipState == EEquipState::Utility_Tertiary)
	{
		EquipmentSlotType = EEquipmentSlotType::Utility_Tertiary;
	}
	else if (EquipState == EEquipState::Utility_Quaternary)
	{
		EquipmentSlotType = EEquipmentSlotType::Utility_Quaternary;
	}
	
	return EquipmentSlotType;
}

void UEquipmentManagerComponent::EquipByQuickSlot(EEquipState NewEquipState)
{
	if (HasAuthority() == false)
		return;

	if (NewEquipState == EEquipState::Count)
		return;

	SetCurrentEquipState(NewEquipState);

	if (UInventoryEquipmentManagerComponent* InventoryEquipmentManager = GetInventoryEquipmentManager())
	{
		for (EEquipmentSlotType EquipmentSlotType : GetEquipmentSlotsByEquipState(NewEquipState))
		{
			Equip(EquipmentSlotType, InventoryEquipmentManager->GetItemInstance(EquipmentSlotType));
		}
	}
}

void UEquipmentManagerComponent::UnequipCurrentSlots()
{
	if (HasAuthority() == false)
		return;

	if (CurrentEquipState == EEquipState::Count)
		return;

	for (EEquipmentSlotType EquipmentSlotType : GetEquipmentSlotsByEquipState(CurrentEquipState))
	{
		Unequip(EquipmentSlotType);
	}
}

void UEquipmentManagerComponent::ChangeEquipState(EEquipState NewEquipState)
{
	if (HasAuthority() == false)
		return;

	if (CanChangeEquipState(NewEquipState))
	{
		if (CurrentEquipState == NewEquipState)
		{
			NewEquipState = EEquipState::Unarmed;
		}

		UnequipCurrentSlots();
		EquipByQuickSlot(NewEquipState);
	}
}

bool UEquipmentManagerComponent::CanChangeEquipState(EEquipState NewEquipState)
{
	if (NewEquipState == EEquipState::Count)
		return false;

	if (CurrentEquipState == EEquipState::Unarmed && NewEquipState == EEquipState::Unarmed)
		return false;

	if (CurrentEquipState == NewEquipState)
		return true;

	UInventoryEquipmentManagerComponent* InventoryEquipmentManager = GetInventoryEquipmentManager();
	if (InventoryEquipmentManager == nullptr)
		return false;
	
	return InventoryEquipmentManager->IsEquipmentSlotEmptyByEquipState(NewEquipState) == false;
}

const TArray<EEquipmentSlotType>& UEquipmentManagerComponent::GetEquipmentSlotsByEquipState(EEquipState EquipState)
{
	static const TArray<TArray<EEquipmentSlotType>> EquipmentSlotsByEquipState =
	{
		{ EEquipmentSlotType::Unarmed_LeftHand, EEquipmentSlotType::Unarmed_RightHand},
		{ EEquipmentSlotType::Primary_LeftHand, EEquipmentSlotType::Primary_RightHand, EEquipmentSlotType::Primary_TwoHand},  // Weapon_Primary
		{ EEquipmentSlotType::Utility_Primary},
		{ EEquipmentSlotType::Utility_Secondary},
		{ EEquipmentSlotType::Utility_Tertiary},
		{ EEquipmentSlotType::Utility_Quaternary}
	};

	if (EquipmentSlotsByEquipState.IsValidIndex((int32)EquipState))
	{
		return EquipmentSlotsByEquipState[(int32)EquipState];
	}

	static const TArray<EEquipmentSlotType> EmptySlots;
	return EmptySlots;
}

void UEquipmentManagerComponent::OnRep_ItemTemplateID(int32 PrevItemTemplateID)
{
	/*if (ItemTemplateID == PrevItemTemplateID)
		return;

	if (OwnerCharacter == nullptr)
		return;

	USkeletalMeshComponent* CharacterMeshComponent = OwnerCharacter->GetMesh();
	if (CharacterMeshComponent == nullptr)
		return;

	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);

	if (const UItemFragment_Equipable_Attachment* AttachmentFragment = ItemTemplate.FindFragmentByClass<
		UItemFragment_Equipable_Attachment>())
	{
		FEquipment Data = AttachmentFragment->GetEquipmentInfo();

		if (Data.DrawMontage)
		{
			if (UMontageSystemComponent* MontageComp = OwnerCharacter->FindComponentByClass<UMontageSystemComponent>())
			{
				MontageComp->PlayMontage(Data.DrawMontage);
			}
		}
	}*/
}

void UEquipmentManagerComponent::OnRep_CurrentEquipState(EEquipState PrevEquipState)
{
	BroadcastChangedMessage(PrevEquipState, CurrentEquipState);
}

void UEquipmentManagerComponent::BroadcastChangedMessage(EEquipState PrevEquipState, EEquipState NewEquipState)
{
	if (OnEquipStateChanged.IsBound())
	{
		OnEquipStateChanged.Broadcast(PrevEquipState, NewEquipState);
	}
}

void UEquipmentManagerComponent::Attack()
{
	// EquipmentComponent에서 현재 무기 타입 가져오기
	FAttackData Data = GetAttackInfo();
	GetMontageIndex();
	if (StateComponent->IsIdleMode() == true && !Data.Montages)
	{
		StateComponent->SetActionMode();
		MontageComponent->PlayMontage(Data.Montages);
	}
}

void UEquipmentManagerComponent::GetMontageIndex() const
{
	
	if (ItemTemplateID == INDEX_NONE)
		return;

	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);

	if (UItemFragment_Equipable_Weapon* WeaponFragment = ItemTemplate.FindFragmentByClass<
		UItemFragment_Equipable_Weapon>())
	{
		//WeaponFragment->IncrementMontageIndex();
	}
}

void UEquipmentManagerComponent::SetCurrentEquipState(EEquipState NewEquipState)
{
	if (HasAuthority() && IsNetMode(ENetMode::NM_ListenServer))
	{
		BroadcastChangedMessage(CurrentEquipState, NewEquipState);
	}

	CurrentEquipState = NewEquipState;
}

TArray<FEquipEntry>& UEquipmentManagerComponent::GetAllEntries()
{
	return EquipList.GetAllEntries();
}

void UEquipmentManagerComponent::SetEquipmentActor(EEquipmentSlotType EquipmentSlotType, AEquipmentBase* InEquipmentActor)
{
	if (EquipmentSlotType == EEquipmentSlotType::Count)
		return;

	TArray<FEquipEntry>& Entries = EquipList.Entries;
	const int32 EntryIndex = (int32)EquipmentSlotType;
	if (Entries.IsValidIndex(EntryIndex) == false)
		return;
	
	Entries[EntryIndex].SetEquipmentActor(InEquipmentActor);
}

UInventoryEquipmentManagerComponent* UEquipmentManagerComponent::GetInventoryEquipmentManager() const
{
	if (AEmberPlayerCharacter* EmberPlayer = Cast<AEmberPlayerCharacter>(GetOwner()))
	{
		return EmberPlayer->FindComponentByClass<UInventoryEquipmentManagerComponent>();
	}
	
	return nullptr;
}

UItemInstance* UEquipmentManagerComponent::GetEquippedItemInstance(EEquipmentSlotType EquipmentSlotType) const
{
	if (EquipmentSlotType == EEquipmentSlotType::Count)
		return nullptr;

	const TArray<FEquipEntry>& Entries = EquipList.Entries;
	const int32 EntryIndex = (int32)EquipmentSlotType;
	return Entries.IsValidIndex(EntryIndex) ? Entries[EntryIndex].GetItemInstance() : nullptr;
}

FAttackData UEquipmentManagerComponent::GetAttackInfo() const
{
	FAttackData AttackInfo;

	if (ItemTemplateID == INDEX_NONE)
		return AttackInfo;

	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);

	if (UItemFragment_Equipable_Weapon* WeaponFragment = ItemTemplate.FindFragmentByClass<
		UItemFragment_Equipable_Weapon>())
	{
		AttackInfo = WeaponFragment->GetAttackInfo();
	}

	return AttackInfo;
}
