// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_RewardItem.h"

#include "EmberPlayerController.h"
#include "ItemInstance.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameInfo/GameplayTags.h"
#include "Managers/ItemManagerComponent.h"

UEmberGameplayAbility_RewardItem::UEmberGameplayAbility_RewardItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::OnSpawn;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
}

void UEmberGameplayAbility_RewardItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EmberGameplayTags::GameplayEvent_Reward_Item, nullptr, false, true))
	{
		GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnRewardItem);
		GameplayEventTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_RewardItem::OnRewardItem(FGameplayEventData TriggerEventData)
{
	if (HasAuthority(&CurrentActivationInfo) == false)
		return;

	UItemInstance*  RewardItemInstance =  Cast<UItemInstance>(const_cast<UObject*>(TriggerEventData.OptionalObject.Get()));;
	if (RewardItemInstance == nullptr)
		return;

	AEmberPlayerController* PlayerController = GetEmberPlayerControllerFromActorInfo();
	if (PlayerController == nullptr)
		return;

	UItemManagerComponent* ItemManager = PlayerController->GetComponentByClass<UItemManagerComponent>();
	if (ItemManager == nullptr)
		return;

	// TODO : 아이템 획득시 UI표시 구현

	ItemManager->TryAutoPickupItem(RewardItemInstance);
}
