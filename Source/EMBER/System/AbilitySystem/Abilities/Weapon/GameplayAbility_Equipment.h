// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "GameplayAbility_Equipment.generated.h"

class UItemInstance;
class AEquipmentBase;

UCLASS()
class EMBER_API UGameplayAbility_Equipment : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Equipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UItemInstance* GetEquipmentItemInstance(const AEquipmentBase* InEquipmentActor) const;
	int32 GetEquipmentStatValue(FGameplayTag InStatTag, const AEquipmentBase* InEquipmentActor) const;
};
