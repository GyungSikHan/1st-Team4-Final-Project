// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "GameFramework/Actor.h"
#include "EquipmentBase.generated.h"

class UBoxComponent;
class UArrowComponent;
class UItemInstance;

UCLASS(Abstract, BlueprintType)
class EMBER_API AEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:
	AEquipmentBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	void Init(int32 InTemplateID, EEquipmentSlotType InEquipmentSlotType);
	
protected:
	//~AActor Overrides
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of Overrides

public:
	int32 GetItemTemplateID() const { return ItemTemplateID; }
	EEquipmentSlotType GetEquipmentSlotType() const { return EquipmentSlotType; }
	
private:
	void ProcessEquip();
	
private:
	UFUNCTION()
	void OnRep_ItemTemplateID();
	
	UFUNCTION()
	void OnRep_EquipmentSlotType();

	void HandleReplicatedEquipment();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> ArrowComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> TraceDebugCollision;
	
protected:
	UPROPERTY(ReplicatedUsing=OnRep_ItemTemplateID)
	int32 ItemTemplateID = INDEX_NONE;
	
	UPROPERTY(ReplicatedUsing=OnRep_EquipmentSlotType)
	EEquipmentSlotType EquipmentSlotType = EEquipmentSlotType::Count;

private:
	UPROPERTY()
	bool bInitialized = false;

	UPROPERTY(EditDefaultsOnly, Category=Info)
	bool bEnableControlRotation = false;
};
