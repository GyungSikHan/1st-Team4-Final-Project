// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"

#include "GameFlag.h"
#include "InventoryEquipmentManagerComponent.h"
#include "ItemTemplate.h"
#include "ItemInstance.h"
//#include "ToolMenusEditor.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "UI/Data/EmberItemData.h"

UItemInstance* FInventoryEntry::Init(int32 InItemTemplateID, int32 InItemCount, EItemRarity InItemRarity)
{
	check(InItemTemplateID > 0 && InItemCount > 0 && InItemRarity != EItemRarity::Count);
	
	UItemInstance* NewItemInstance = NewObject<UItemInstance>();
	NewItemInstance->Init(InItemTemplateID, InItemRarity);
	Init(NewItemInstance, InItemCount);
	
	return NewItemInstance;
}

void FInventoryEntry::Init(UItemInstance* InItemInstance, int32 InItemCount)
{
	check(InItemInstance && InItemCount > 0);
	
	ItemInstance = InItemInstance;
	
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
	ItemCount = FMath::Clamp(InItemCount, 1, ItemTemplate.MaxStackCount);
}

UItemInstance* FInventoryEntry::Reset()
{
	UItemInstance* RemovedItemInstance = ItemInstance;
	ItemInstance = nullptr;
	ItemCount = 0;
	
	return RemovedItemInstance;
}

bool FInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryEntry, FInventoryList>(Entries, DeltaParams, *this);
}

void FInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	const FIntPoint& InventorySlotCount = InventoryManager->GetInventorySlotCount();

	for (int32 AddedIndex : AddedIndices)
	{
		FInventoryEntry& Entry = Entries[AddedIndex];
		if (Entry.ItemInstance)
		{
			const FIntPoint ItemSlotPos = FIntPoint(AddedIndex % InventorySlotCount.X, AddedIndex / InventorySlotCount.X);
			BroadcastChangedMessage(ItemSlotPos, Entry.ItemInstance, Entry.ItemCount);
		}
	}
}

void FInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	TArray<int32> AddedIndices;
	AddedIndices.Reserve(FinalSize);

	const FIntPoint& InventorySlotCount = InventoryManager->GetInventorySlotCount();
	
	for (int32 ChangedIndex : ChangedIndices)
	{
		FInventoryEntry& Entry = Entries[ChangedIndex];
		if (Entry.ItemInstance)
		{
			AddedIndices.Add(ChangedIndex);
		}
		else
		{
			const FIntPoint ItemSlotPos = FIntPoint(ChangedIndex % InventorySlotCount.X, ChangedIndex / InventorySlotCount.X);
			BroadcastChangedMessage(ItemSlotPos, nullptr, 0);
		}
	}

	for (int32 AddedIndex : AddedIndices)
	{
		FInventoryEntry& Entry = Entries[AddedIndex];
		const FIntPoint ItemSlotPos = FIntPoint(AddedIndex % InventorySlotCount.X, AddedIndex / InventorySlotCount.X);
		BroadcastChangedMessage(ItemSlotPos, Entry.ItemInstance, Entry.ItemCount);
	}
}

void FInventoryList::CustomMarkItemDirty(AActor* Owner, FInventoryEntry& ToEntry, const FIntPoint& ToItemSlotPos)
{
	MarkItemDirty(ToEntry);

	if (Owner && Owner->IsNetMode(NM_ListenServer))
	{
		BroadcastChangedMessage(ToItemSlotPos, ToEntry.GetItemInstance(), ToEntry.GetItemCount());
	}
}

void FInventoryList::BroadcastChangedMessage(const FIntPoint& ItemSlotPos, UItemInstance* ItemInstance, int32 ItemCount)
{
	if (InventoryManager->OnInventoryEntryChanged.IsBound())
	{
		InventoryManager->OnInventoryEntryChanged.Broadcast(ItemSlotPos, ItemInstance, ItemCount);
	}
}

UInventoryManagerComponent::UInventoryManagerComponent(const FObjectInitializer& FObjectInitializer) : Super(FObjectInitializer), InventoryList(this)
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UInventoryManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		TArray<FInventoryEntry>& Entries = InventoryList.Entries;
		Entries.SetNum(InventorySlotCount.X * InventorySlotCount.Y);
	
		for (FInventoryEntry& Entry : Entries)
		{
			InventoryList.MarkItemDirty(Entry);
		}

		SlotChecks.SetNumZeroed(InventorySlotCount.X * InventorySlotCount.Y);
	}
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
	DOREPLIFETIME(ThisClass, SlotChecks);
}

bool UInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	for (FInventoryEntry& Entry : InventoryList.Entries)
	{
		UItemInstance* ItemInstance = Entry.ItemInstance;
		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}
	
	return bWroteSomething;
}

void UInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
	
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryEntry& Entry : InventoryList.Entries)
		{
			UItemInstance* ItemInstance = Entry.GetItemInstance();
			if (IsValid(ItemInstance))
			{
				AddReplicatedSubObject(ItemInstance);
			}
		}
	}
}

int32 UInventoryManagerComponent::CanMoveOrMergeItem(UInventoryManagerComponent* OtherComponent, const FIntPoint& FromItemSlotPos, const FIntPoint& ToItemSlotPos) const
{
	if (OtherComponent == nullptr)
		return 0;

	const FIntPoint& FromInventorySlotCount = OtherComponent->GetInventorySlotCount();
	if (FromItemSlotPos.X < 0 || FromItemSlotPos.Y < 0 || FromItemSlotPos.X >= FromInventorySlotCount.X || FromItemSlotPos.Y >= FromInventorySlotCount.Y)
		return 0;

	if (ToItemSlotPos.X < 0 || ToItemSlotPos.Y < 0 || ToItemSlotPos.X >= InventorySlotCount.X || ToItemSlotPos.Y >= InventorySlotCount.Y)
		return 0;
	
	const UItemInstance* FromItemInstance = OtherComponent->GetItemInstance(FromItemSlotPos);
	const int32 FromItemCount = OtherComponent->GetItemCount(FromItemSlotPos);
	
	if (IsSameComponent(OtherComponent) && FromItemSlotPos == ToItemSlotPos)
		return FromItemCount;
	
	if (FromItemInstance == nullptr || FromItemCount <= 0)
		return 0;

	const UItemInstance* ToItemInstance = GetItemInstance(ToItemSlotPos);
	const int32 ToItemCount = GetItemCount(ToItemSlotPos);

	const int32 FromTemplateID = FromItemInstance->GetItemTemplateID();
	const UItemTemplate& FromItemTemplate = UEmberItemData::Get().FindItemTemplateByID(FromTemplateID);
	
	if (ToItemInstance)
	{
		const int32 ToTemplateID = ToItemInstance->GetItemTemplateID();
		if (FromTemplateID != ToTemplateID)
			return 0;

		if (FromItemInstance->GetItemRarity() != ToItemInstance->GetItemRarity())
			return 0;
		
		if (FromItemTemplate.MaxStackCount < 2)
			return 0;

		return FMath::Min(FromItemCount + ToItemCount, FromItemTemplate.MaxStackCount) - ToItemCount;
	}
	
	const FIntPoint& FromItemSlotCount = FromItemTemplate.SlotCount;
	if (ToItemSlotPos.X + FromItemSlotCount.X > InventorySlotCount.X || ToItemSlotPos.Y + FromItemSlotCount.Y > InventorySlotCount.Y)
		return 0;
	
	if (IsSameComponent(OtherComponent))
	{
		TArray<bool> TempSlotChecks = SlotChecks;
		MarkSlotChecks(TempSlotChecks, false, FromItemSlotPos, FromItemSlotCount);
		
		return IsEmpty(TempSlotChecks, ToItemSlotPos, FromItemSlotCount) ? FromItemCount : 0;
	}

	return IsEmpty(ToItemSlotPos, FromItemSlotCount) ? FromItemCount : 0;
}

int32 UInventoryManagerComponent::CanMoveOrMergeItem(UInventoryEquipmentManagerComponent* FromInventoryEquipmentManager, EEquipmentSlotType FromEquipmentSlotType, const FIntPoint& ToItemSlotPos) const
{
	if (FromInventoryEquipmentManager == nullptr)
		return 0;

	if (FromEquipmentSlotType == EEquipmentSlotType::Count)
		return 0;
	
	if (ToItemSlotPos.X < 0 || ToItemSlotPos.Y < 0 || ToItemSlotPos.X >= InventorySlotCount.X || ToItemSlotPos.Y >= InventorySlotCount.Y)
		return 0;

	// 장비창에서 드래그한 아이템이 유효한지 확인
	const UItemInstance* FromItemInstance = FromInventoryEquipmentManager->GetItemInstance(FromEquipmentSlotType);
	const int32 FromItemCount = FromInventoryEquipmentManager->GetItemCount(FromEquipmentSlotType);

	if (FromItemInstance == nullptr || FromItemCount <= 0)
		return 0;
	
	// 인벤토리의 드래그한 위치에 아이템이 있는지 확인
	const UItemInstance* ToItemInstance = GetItemInstance(ToItemSlotPos);
	const int32 ToItemCount = GetItemCount(ToItemSlotPos);
	
	const int32 FromTemplateID = FromItemInstance->GetItemTemplateID();
	const UItemTemplate& FromItemTemplate = UEmberItemData::Get().FindItemTemplateByID(FromTemplateID);

	if (ToItemInstance)
	{
		const int32 ToTemplateID = ToItemInstance->GetItemTemplateID();
		if (FromTemplateID != ToTemplateID)
			return 0;

		if (FromItemInstance->GetItemRarity() != ToItemInstance->GetItemRarity())
			return 0;
		
		if (FromItemTemplate.MaxStackCount < 2)
			return 0;

		return FMath::Min(FromItemCount + ToItemCount, FromItemTemplate.MaxStackCount) - ToItemCount;
	}
	else
	{
		const FIntPoint& FromItemSlotCount = FromItemTemplate.SlotCount;
		if (ToItemSlotPos.X + FromItemSlotCount.X > InventorySlotCount.X || ToItemSlotPos.Y + FromItemSlotCount.Y > InventorySlotCount.Y)
			return 0;

		return IsEmpty(ToItemSlotPos, FromItemSlotCount) ? FromItemCount : 0;
	}
}

int32 UInventoryManagerComponent::CanAddItem(UItemInstance* ItemInstance, TArray<FIntPoint>& OutToItemSlotPoses, TArray<int32>& OutToItemCounts)
{
	if (ItemInstance == nullptr)
		return 0;

	int32 ItemTemplateID = ItemInstance->GetItemTemplateID();
	int32 ItemCount = ItemInstance->GetItemCount();
	EItemRarity ItemRarity = ItemInstance->GetItemRarity();

	return CanAddItem(ItemTemplateID, ItemRarity, ItemCount, OutToItemSlotPoses, OutToItemCounts);
}


int32 UInventoryManagerComponent::CanAddItem(int32 ItemTemplateID, EItemRarity ItemRarity, int32 ItemCount, TArray<FIntPoint>& OutToItemSlotPoses, TArray<int32>& OutToItemCounts) const
{
	OutToItemSlotPoses.Reset();
	OutToItemCounts.Reset();

	if (ItemTemplateID <= 0 || ItemRarity == EItemRarity::Count || ItemCount <= 0)
		return 0;

	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);
	int32 LeftItemCount = ItemCount;

	// 아이템 중첩 개수가 2개 이상인 경우
	// 소지 아이템들을 탐색하여 개수를 증가시킨다. 
	if (ItemTemplate.MaxStackCount > 1)
	{
		const TArray<FInventoryEntry>& ToEntries = GetAllEntries();

		for (int32 i = 0; i < ToEntries.Num(); i++)
		{
			const FInventoryEntry& ToEntry = ToEntries[i];
			const UItemInstance* ToItemInstance = ToEntry.GetItemInstance();
			const int32 ToItemCount = ToEntry.GetItemCount();

			if (ToItemInstance == nullptr)
				continue;

			if (ToItemInstance->GetItemTemplateID() != ItemTemplateID)
				continue;

			if (ToItemInstance->GetItemRarity() != ItemRarity)
				continue;

			if (int32 AddCount = FMath::Min(ToItemCount + LeftItemCount ,ItemTemplate.MaxStackCount) - ToItemCount)
			{
				OutToItemSlotPoses.Emplace(i % InventorySlotCount.X, i / InventorySlotCount.X);
				OutToItemCounts.Emplace(AddCount);
				LeftItemCount -= AddCount;

				if (LeftItemCount == 0)
					return ItemCount;
			}
		}
	}

	// 인벤토리 빈 공간을 확인하여 아이템을 추가한다.
	const FIntPoint& ItemSlotCount = ItemTemplate.SlotCount;
	TArray<bool> TempSlotChecks = SlotChecks;

	const FIntPoint StartSlotPos = FIntPoint::ZeroValue;
	const FIntPoint EndSlotPos = InventorySlotCount - ItemSlotCount;

	for (int32 y = StartSlotPos.Y; y <= EndSlotPos.Y; y++)
	{
		for (int32 x = StartSlotPos.X; x <= EndSlotPos.X; x++)
		{
			int32 Index = y * InventorySlotCount.X + x;
			if (TempSlotChecks.IsValidIndex(Index) == false || TempSlotChecks[Index])
				continue;

			FIntPoint ItemSlotPos = FIntPoint(x, y);
			if (IsEmpty(TempSlotChecks, ItemSlotPos, ItemSlotCount))
			{
				MarkSlotChecks(TempSlotChecks, true, ItemSlotPos, ItemSlotCount);
				
				int32 AddCount = FMath::Min(LeftItemCount, ItemTemplate.MaxStackCount);
				OutToItemSlotPoses.Emplace(ItemSlotPos);
				OutToItemCounts.Emplace(AddCount);
				
				LeftItemCount -= AddCount;
				
				if (LeftItemCount == 0)
					return ItemCount;
			}
		}
	}

	return ItemCount - LeftItemCount;
}

int32 UInventoryManagerComponent::TryAddItemByRarity(TSubclassOf<UItemTemplate> ItemTemplateClass, EItemRarity ItemRarity, int32 ItemCount)
{
	check(GetOwner()->HasAuthority());

	if (ItemTemplateClass == nullptr || ItemRarity == EItemRarity::Count || ItemCount <= 0)
		return 0;

	int32 ItemTemplateID = UEmberItemData::Get().FindItemTemplateIDByClass(ItemTemplateClass);
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);

	TArray<FIntPoint> ToItemSlotPoses;
	TArray<int32> ToItemCounts;

	int32 AddableItemCount = CanAddItem(ItemTemplateID, ItemRarity, ItemCount, ToItemSlotPoses, ToItemCounts);
	if (AddableItemCount > 0)
	{
		TArray<UItemInstance*> AddedItemInstances;
		
		for (int32 i = 0; i < ToItemSlotPoses.Num(); i++)
		{
			const FIntPoint& ToItemSlotPos = ToItemSlotPoses[i];
			const int32 ToItemCount = ToItemCounts[i];

			const int32 ToIndex = ToItemSlotPos.Y * InventorySlotCount.X + ToItemSlotPos.X;
			FInventoryEntry& ToEntry = InventoryList.Entries[ToIndex];

			if (ToEntry.ItemInstance)
			{
				ToEntry.ItemCount += ToItemCount;
			}
			else
			{
				AddedItemInstances.Add(ToEntry.Init(ItemTemplateID, ToItemCount, ItemRarity));
				MarkSlotChecks(true, ToItemSlotPos, ItemTemplate.SlotCount);
			}

			InventoryList.CustomMarkItemDirty(GetOwner(), ToEntry, ToItemSlotPos);
		}

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			for (UItemInstance* AddedItemInstance : AddedItemInstances)
			{
				if (AddedItemInstance)
				{
					AddReplicatedSubObject(AddedItemInstance);
				}
			}
		}
		
		if (AddableItemCount > 0)
		{
			OnItemAdded.Broadcast(ItemTemplateClass, AddableItemCount);
		}
		
		return AddableItemCount;
	}

	return 0;
}

bool UInventoryManagerComponent::FindItemByID(int32 FindItemID, TArray<FFindItemData>& OutFindItemList)
{
	check(GetOwner()->HasAuthority());

	OutFindItemList.Empty();
	
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(FindItemID);
	const TArray<FInventoryEntry>& Entries = GetAllEntries();

	TArray<bool> Visibles;
	Visibles.Init(false, Entries.Num());
	
	for (int32 idx = 0; idx < Entries.Num(); idx++)
	{
		if (Visibles[idx])
			continue;
		
		if (UItemInstance* ItemInstance = Entries[idx].GetItemInstance())
		{
			int32 EntryItemID = ItemInstance->GetItemTemplateID();
			int32 EntryItemCount = ItemInstance->GetItemCount();
			
			if (FindItemID == EntryItemID)
			{
				FIntPoint EntryItemPos(idx % InventorySlotCount.X, idx / InventorySlotCount.X);
				
				FFindItemData FindItemData;
				FindItemData.ItemID = EntryItemID;
				FindItemData.ItemCount = EntryItemCount;
				FindItemData.ItemSlotPos = EntryItemPos;
					
				OutFindItemList.Emplace(FindItemData);

				MarkSlotChecks(Visibles, true, EntryItemPos, ItemTemplate.SlotCount);
			}
		}
	}
	
	return OutFindItemList.IsEmpty() == false;
}

void UInventoryManagerComponent::AddItem_Unsafe(const FIntPoint& ItemSlotPos, UItemInstance* ItemInstance, int32 ItemCount)
{
	check(GetOwner()->HasAuthority());
    
	const int32 Index = ItemSlotPos.Y * InventorySlotCount.X + ItemSlotPos.X;
	FInventoryEntry& Entry = InventoryList.Entries[Index];
    
	if (Entry.GetItemInstance())
	{
		Entry.ItemCount += ItemCount;
		InventoryList.CustomMarkItemDirty(GetOwner() ,Entry, ItemSlotPos);
	}
	else
	{
		if (ItemInstance == nullptr)
			return;
       
		const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
       
		Entry.Init(ItemInstance, ItemCount);
       
		if (IsReadyForReplication() && ItemInstance)
		{
			AddReplicatedSubObject(ItemInstance);
		}

		MarkSlotChecks(true, ItemSlotPos, ItemTemplate.SlotCount);
		InventoryList.CustomMarkItemDirty(GetOwner() ,Entry, ItemSlotPos);
	}

	if (ItemInstance && ItemCount > 0)
	{
		const int32 ItemID = ItemInstance->GetItemTemplateID();
        
		const UItemTemplate* ItemTemplateCDO = &UEmberItemData::Get().FindItemTemplateByID(ItemID);
		if (ItemTemplateCDO)
		{
			TSubclassOf<UItemTemplate> ItemTemplateClass = ItemTemplateCDO->GetClass();
			if (ItemTemplateClass)
			{
				OnItemAdded.Broadcast(ItemTemplateClass, ItemCount);
			}
		}
	}
}

UItemInstance* UInventoryManagerComponent::RemoveItem_Unsafe(const FIntPoint& ItemSlotPos, int32 ItemCount)
{
	check(GetOwner()->HasAuthority());
	
	const int32 Index = ItemSlotPos.Y * InventorySlotCount.X + ItemSlotPos.X;
	FInventoryEntry& Entry = InventoryList.Entries[Index];
	UItemInstance* ItemInstance = Entry.GetItemInstance();
	
	Entry.ItemCount -= ItemCount;
	if (Entry.GetItemCount() <= 0)
	{
		const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
		MarkSlotChecks(false, ItemSlotPos, ItemTemplate.SlotCount);
		
		UItemInstance* RemovedItemInstance = Entry.Reset();
		if (IsUsingRegisteredSubObjectList() && RemovedItemInstance)
		{
			RemoveReplicatedSubObject(RemovedItemInstance);
		}
	}
	
	InventoryList.CustomMarkItemDirty(GetOwner() ,Entry, ItemSlotPos);
	return ItemInstance;
}

bool UInventoryManagerComponent::IsEmpty(const FIntPoint& ItemSlotPos, const FIntPoint& ItemSlotCount) const
{
	return IsEmpty(SlotChecks, ItemSlotPos, ItemSlotCount);
}

bool UInventoryManagerComponent::IsEmpty(const TArray<bool>& InSlotChecks, const FIntPoint& ItemSlotPos, const FIntPoint& ItemSlotCount) const
{
	if (ItemSlotPos.X < 0 || ItemSlotPos.Y < 0)
		return false;

	if (ItemSlotPos.X + ItemSlotCount.X > InventorySlotCount.X || ItemSlotPos.Y + ItemSlotCount.Y > InventorySlotCount.Y)
		return false;

	const FIntPoint StartSlotPos = ItemSlotPos;
	const FIntPoint EndSlotPos = ItemSlotPos + ItemSlotCount;

	for (int32 y = StartSlotPos.Y; y < EndSlotPos.Y; y++)
	{
		for (int32 x = StartSlotPos.X; x < EndSlotPos.X; x++)
		{
			int32 Index = y * InventorySlotCount.X + x;
			if (InSlotChecks.IsValidIndex(Index) == false || InSlotChecks[Index])
				return false;
		}
	}
	
	return true;
}

bool UInventoryManagerComponent::IsSameComponent(const UInventoryManagerComponent* OtherComponent) const
{
	return this == OtherComponent;
}

UItemInstance* UInventoryManagerComponent::GetItemInstance(const FIntPoint& ItemSlotPos) const
{
	if (ItemSlotPos.X < 0 || ItemSlotPos.Y < 0 || ItemSlotPos.X >= InventorySlotCount.X || ItemSlotPos.Y >= InventorySlotCount.Y)
		return nullptr;

	const TArray<FInventoryEntry>& Entries = InventoryList.GetAllEntries();
	const int32 EntryIndex = ItemSlotPos.Y * InventorySlotCount.X + ItemSlotPos.X;
	const FInventoryEntry& Entry = Entries[EntryIndex];
	
	return Entry.GetItemInstance();
}

int32 UInventoryManagerComponent::GetItemCount(const FIntPoint& ItemSlotPos) const
{
	if (ItemSlotPos.X < 0 || ItemSlotPos.Y < 0 || ItemSlotPos.X >= InventorySlotCount.X || ItemSlotPos.Y >= InventorySlotCount.Y)
		return 0;

	const TArray<FInventoryEntry>& Entries = InventoryList.GetAllEntries();
	const int32 EntryIndex = ItemSlotPos.Y * InventorySlotCount.X + ItemSlotPos.X;
	const FInventoryEntry& Entry = Entries[EntryIndex];

	return Entry.GetItemCount();
}

const TArray<FInventoryEntry>& UInventoryManagerComponent::GetAllEntries() const
{
	return InventoryList.GetAllEntries();
}

void UInventoryManagerComponent::MarkSlotChecks(TArray<bool>& InSlotChecks, bool bIsUsing, const FIntPoint& ItemSlotPos, const FIntPoint& ItemSlotCount) const
{
	if (ItemSlotPos.X < 0 || ItemSlotPos.Y < 0)
		return;

	if (ItemSlotPos.X + ItemSlotCount.X > InventorySlotCount.X || ItemSlotPos.Y + ItemSlotCount.Y > InventorySlotCount.Y)
		return;

	const FIntPoint StartSlotPos = ItemSlotPos;
	const FIntPoint EndSlotPos = ItemSlotPos + ItemSlotCount;

	for (int32 y = StartSlotPos.Y; y < EndSlotPos.Y; y++)
	{
		for (int32 x = StartSlotPos.X; x < EndSlotPos.X; x++)
		{
			int32 Index = y * InventorySlotCount.X + x;
			if (InSlotChecks.IsValidIndex(Index))
			{
				InSlotChecks[Index] = bIsUsing;
			}
		}
	}
}

void UInventoryManagerComponent::MarkSlotChecks(bool bIsUsing, const FIntPoint& ItemSlotPos, const FIntPoint& ItemSlotCount)
{
	MarkSlotChecks(SlotChecks, bIsUsing, ItemSlotPos, ItemSlotCount);
}
