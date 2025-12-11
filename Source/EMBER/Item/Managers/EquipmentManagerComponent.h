// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "GameFlag.h"
#include "Components/PawnComponent.h"
#include "System/AbilitySystem/Data/EmberAbilitySet.h"

#include "EquipmentManagerComponent.generated.h"

class UInventoryEquipmentManagerComponent;
enum class EEquipmentSlotType : uint8;
enum class EWeaponSlotType : uint8;
class UEquipmentManagerComponent;
class UItemTemplate;
class UItemInstance;
class AEquipmentBase;
struct FAttackData;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipStateChanged, EEquipState, EEquipState);

USTRUCT(BlueprintType)
struct FEquipEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	UItemInstance* GetItemInstance() const { return ItemInstance; }

	void SetEquipmentActor(AEquipmentBase* InEquipmentActor) { SpawnedEquipmentActor = InEquipmentActor; }
	AEquipmentBase* GetEquipmentActor() const { return SpawnedEquipmentActor; }
	
private:
	void Init(UItemInstance* InItemInstance);

	void Equip();
	void Unequip();

public:
	UPROPERTY(NotReplicated)
	TObjectPtr<UEquipmentManagerComponent> EquipmentManager;

private:
	friend UEquipmentManagerComponent;
	friend FEquipList;
	
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;

private:
	UPROPERTY(NotReplicated)
	FEmberAbilitySet_GrantedHandles BaseAbilitySetHandles;

	UPROPERTY(NotReplicated)
	FActiveGameplayEffectHandle BaseStatHandle;

private:
	UPROPERTY(NotReplicated)
	EEquipmentSlotType EquipmentSlotType = EEquipmentSlotType::Count;

private:
	UPROPERTY(NotReplicated)
	TObjectPtr<AEquipmentBase> SpawnedEquipmentActor;
};

USTRUCT(BlueprintType)
struct FEquipList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FEquipList() : EquipmentManager(nullptr) {}
	FEquipList(UEquipmentManagerComponent* InOwnerComponent) : EquipmentManager(InOwnerComponent) {}

public:
	//~FFastArraySerializer Override
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer Override
	
private:
	void Equip(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance);
	void Unequip(EEquipmentSlotType EquipmentSlotType);
	void Equip_UnSafe(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance);
	void Unequip_UnSafe(EEquipmentSlotType EquipmentSlotType);

public:
	TArray<FEquipEntry>& GetAllEntries() { return Entries; }
	
private:
	friend UEquipmentManagerComponent;
	
	UPROPERTY()
	TArray<FEquipEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UEquipmentManagerComponent> EquipmentManager;
};

template<>
struct TStructOpsTypeTraits<FEquipList> : public TStructOpsTypeTraitsBase2<FEquipList>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMBER_API UEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	
protected:
	//~UPawnComponent Overrides
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UPawnComponent overrides
	
public:
	void AddEquipment(EWeaponSlotType WeaponSlotType, TSubclassOf<UItemTemplate> ItemTemplateClass);
	void Equip(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance);
	void Unequip(EEquipmentSlotType EquipmentSlotType);

protected:
	void Equip_Armor(UItemInstance* ItemInstance);
	void Unequip_Armor(UItemInstance* ItemInstance);

public:
	static EEquipState ConvertToEquipState(EUtilitySlotType UtilitySlotType);
	static EEquipmentSlotType ConvertToEquipmentSlotType(EWeaponHandType WeaponHandType, EEquipState EquipState);

/* EquipState */
public:
	void EquipByQuickSlot(EEquipState NewEquipState);
	void UnequipCurrentSlots();
	
	void ChangeEquipState(EEquipState NewEquipState);
	bool CanChangeEquipState(EEquipState NewEquipState);
	static const TArray<EEquipmentSlotType>& GetEquipmentSlotsByEquipState(EEquipState EquipState);
	EEquipState GetCurrentEquipState() const { return CurrentEquipState; }
	void SetCurrentEquipState(EEquipState NewEquipState);
	
public:
	TArray<FEquipEntry>& GetAllEntries();
	
	void SetEquipmentActor(EEquipmentSlotType EquipmentSlotType, AEquipmentBase* InEquipmentActor);

	AEquipmentBase* GetHandEquipment() const;
	AEquipmentBase* GetEquippedActor(EWeaponHandType WeaponHandType) const;
	UInventoryEquipmentManagerComponent* GetInventoryEquipmentManager() const;
	UItemInstance* GetEquippedItemInstance(EEquipmentSlotType EquipmentSlotType) const;
	
public:
	void Attack();
	UFUNCTION()
	FAttackData GetAttackInfo() const;
	UFUNCTION()
	void GetMontageIndex() const;

private:
	UFUNCTION()
	void OnRep_ItemTemplateID(int32 PrevItemTemplateID);

	UFUNCTION()
	void OnRep_CurrentEquipState(EEquipState PrevEquipState);

	void BroadcastChangedMessage(EEquipState PrevEquipState, EEquipState NewEquipState);

public:
	FOnEquipStateChanged OnEquipStateChanged;;	
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_ItemTemplateID)
	int32 ItemTemplateID = INDEX_NONE;

	UPROPERTY()
	AEquipmentBase* SpawnedWeapon;

	UPROPERTY()
	TObjectPtr<AEquipmentBase> SpawnedHandEquipment;

private:
	UPROPERTY(Replicated)
	FEmberAbilitySet_GrantedHandles BaseAbilitySetHandles;

	UPROPERTY(Replicated)
	FActiveGameplayEffectHandle BaseStatHandle;

private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentEquipState)
	EEquipState CurrentEquipState;

	UPROPERTY(Replicated)
	FEquipList EquipList;
	
private:
	ACharacter* OwnerCharacter;
	TObjectPtr<class UArmorComponent> ArmorComponent;
	TObjectPtr<class UMontageSystemComponent> MontageComponent;
	TObjectPtr<class UC_StateComponent> StateComponent;

	bool bIsClick;
};
