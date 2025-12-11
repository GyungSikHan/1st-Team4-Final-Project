// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Equipment.h"

#include "EmberPlayerCharacter.h"
#include "ItemInstance.h"
#include "Equipment/EquipmentBase.h"
#include "Managers/EquipmentManagerComponent.h"

UGameplayAbility_Equipment::UGameplayAbility_Equipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bServerRespectsRemoteAbilityCancellation = false;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
}

UItemInstance* UGameplayAbility_Equipment::GetEquipmentItemInstance(const AEquipmentBase* InEquipmentActor) const
{
	if (InEquipmentActor == nullptr)
		return nullptr;

	AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo();
	if (EmberCharacter == nullptr)
		return nullptr;
	
	UEquipmentManagerComponent* EquipManager = EmberCharacter->FindComponentByClass<UEquipmentManagerComponent>();
	if (EquipManager == nullptr)
		return nullptr;

	return EquipManager->GetEquippedItemInstance(InEquipmentActor->GetEquipmentSlotType());
}

int32 UGameplayAbility_Equipment::GetEquipmentStatValue(FGameplayTag InStatTag, const AEquipmentBase* InEquipmentActor) const
{
	if (InStatTag.IsValid() && InEquipmentActor)
	{
		if (UItemInstance* ItemInstance = GetEquipmentItemInstance(InEquipmentActor))
		{
			return ItemInstance->GetStatCountByTag(InStatTag);
		}
	}
	return 0;
}
