// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EMBER.h"
#include "CoreMinimal.h"
#include "ItemFragment_Equipable.h"
#include "ItemFragment_Equipable_Attachment.generated.h"

class AEquipmentBase;

USTRUCT(BlueprintType)
struct FItemAttachInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEquipmentBase> SpawnEquipmentClass;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocket;

	UPROPERTY(EditDefaultsOnly)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS(Abstract, Const)
class EMBER_API UItemFragment_Equipable_Attachment : public UItemFragment_Equipable
{
	GENERATED_BODY()

public:
	UItemFragment_Equipable_Attachment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	UPROPERTY(EditDefaultsOnly)
	FEquipment EquipmentInfo;
	
public:
	UPROPERTY(EditDefaultsOnly)
	EWeaponHandType WeaponHandType = EWeaponHandType::Count;
	
	UPROPERTY(EditDefaultsOnly)
	FItemAttachInfo ItemAttachInfo;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

public:
	UFUNCTION(BlueprintCallable)
	FEquipment GetEquipmentInfo() const;
};
