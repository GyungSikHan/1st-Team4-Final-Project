// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EquipmentManagerComponent.h"
#include "GameData.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "InventoryEquipmentManagerComponent.generated.h"

class UItemInstance;
class UItemFragment_Equipable;
class UEquipmentManagerComponent;
class UInventoryManagerComponent;
class UInventoryEquipmentManagerComponent;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentEntryChanged, EEquipmentSlotType, UItemInstance*, int32/*ItemCount*/);

USTRUCT(BlueprintType)
struct FEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

private:
	void Init(UItemInstance* InItemInstance, int32 InItemCount);
	UItemInstance* Reset();
	
public:
	UItemInstance* GetItemInstance() const { return ItemInstance; }
	int32 GetItemCount() const { return ItemCount; }

private:
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;

	UPROPERTY()
	int32 ItemCount = 0;
	
private:
	friend FEquipmentList;
	friend UInventoryEquipmentManagerComponent;
	
	UPROPERTY(NotReplicated)
	EEquipmentSlotType EquipmentSlotType = EEquipmentSlotType::Count;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UInventoryEquipmentManagerComponent> InventoryEquipmentManager;
};

USTRUCT(BlueprintType)
struct FEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FEquipmentList() : InventoryEquipmentManager(nullptr) {}
	FEquipmentList(UInventoryEquipmentManagerComponent* InInventoryEquipmentManager) : InventoryEquipmentManager(InInventoryEquipmentManager) {}

public:
	//~FFastArraySerializer Overrides
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer Overrides

public:
	void CustomMarkItemDirty(AActor* Owner, FEquipmentEntry& Entry);
	
private:
	void BroadcastChangedMessage(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance, int32 ItemCount);
	
public:
	const TArray<FEquipmentEntry>& GetAllEntries() const { return Entries; }
	
private:
	friend UInventoryEquipmentManagerComponent;
	
	UPROPERTY()
	TArray<FEquipmentEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UInventoryEquipmentManagerComponent> InventoryEquipmentManager;
};

template<>
struct TStructOpsTypeTraits<FEquipmentList> : public TStructOpsTypeTraitsBase2<FEquipmentList>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class EMBER_API UInventoryEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UInventoryEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UPawnComponent Overrides
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UPawnComponent Overrides
	
public:
	void AddEquipment_Unsafe(EEquipmentSlotType EquipmentSlotType, UItemInstance* FromItemInstance, int32 FromItemCount);
	UItemInstance* RemoveEquipment_Unsafe(EEquipmentSlotType EquipmentSlotType, int32 ItemCount);

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddUnarmedEquipments(TSubclassOf<UItemTemplate> RightHandClass);

	void SetEquipment(EEquipmentSlotType EquipmentSlotType, TSubclassOf<UItemTemplate> ItemTemplateClass, EItemRarity ItemRarity, int32 ItemCount, bool bCheckCharacterClass = true);
	
public:
	int32 CanMoveOrMergeEquipment(UInventoryEquipmentManagerComponent* OtherComponent, EEquipmentSlotType FromEquipmentSlotType, EEquipmentSlotType ToEquipmentSlotType) const;
	int32 CanMoveOrMergeEquipment(UInventoryManagerComponent* OtherComponent, const FIntPoint& FromItemSlotPos, EEquipmentSlotType ToEquipmentSlotType) const;

	bool CanSwapEquipment(UInventoryEquipmentManagerComponent* OtherComponent, EEquipmentSlotType FromEquipmentSlotType, EEquipmentSlotType ToEquipmentSlotType) const;
	bool CanSwapEquipment(UInventoryManagerComponent* OtherComponent, const FIntPoint& FromItemSlotPos, EEquipmentSlotType ToEquipmentSlotType, FIntPoint& OutToItemSlotPos);

	int32 CanAddEquipment(int32 FromItemTemplateID, EItemRarity FromItemRarity, int32 FromItemCount, EEquipmentSlotType ToEquipmentSlotType) const;

	bool IsMatchingSlotType(EEquipmentType FromEquipmentType, EEquipmentSlotType ToEquipmentSlotType) const;
	bool IsMatchingSlotType(const UItemFragment_Equipable* FromEquippableFragment, EEquipmentSlotType ToEquipmentSlotType) const;
	bool IsSlotEquipped(EEquipmentSlotType EquipmentSlotType) const;

	bool IsSameEquipState(EEquipmentSlotType EquipmentSlotType, EEquipState WeaponEquipState) const;
	bool IsSameWeaponHandType(EEquipmentSlotType EquipmentSlotType, EWeaponHandType WeaponHandType) const;
	bool IsSameComponent(const UInventoryEquipmentManagerComponent* OtherComponent) const;
	
	bool IsEquipmentSlotEmptyByEquipState(EEquipState EquipState);

public:
	static EUtilitySlotType ConvertToUtilitySlotType(EEquipmentSlotType EquipmentSlotType);
	
public:
	UEquipmentManagerComponent* GetEquipmentManager() const;
	
	UItemInstance* GetItemInstance(EEquipmentSlotType EquipmentSlotType) const;
	UItemInstance* GetItemInstance(const UInventoryEquipmentManagerComponent* OtherComponent, EEquipmentSlotType EquipmentSlotType) const;
	
	const FEquipmentList& GetEquipmentList() const { return EquipmentList; }
	const TArray<FEquipmentEntry>& GetAllEntries() const;
	int32 GetItemCount(EEquipmentSlotType EquipmentSlotType) const;

public:
	FOnEquipmentEntryChanged OnEquipmentEntryChanged;
	
private:
	UPROPERTY(Replicated)
	FEquipmentList EquipmentList;

	EEquipState CurrentEquipState; 
};
