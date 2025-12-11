// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_MeleeAttack.h"

#include "EmberPlayerCharacter.h"
#include "StatusComponent.h"
#include "System/AbilitySystem/EmberAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Equipment/EquipmentBase.h"
#include "GameInfo/GameplayTags.h"
#include "kismet/GameplayStatics.h"

UEmberGameplayAbility_MeleeAttack::UEmberGameplayAbility_MeleeAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	ActivationPolicy = EEmberAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	bServerRespectsRemoteAbilityCancellation = false;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
}

void UEmberGameplayAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AEmberPlayerCharacter* player = Cast<AEmberPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (player != nullptr)
	{
		UStatusComponent* status = player->GetStatusComponent();
		if (status != nullptr)
		{
			if (status->GetStamina() < player->GetAttackAmount())
				return;
			status->UseStamina(player->GetAttackAmount());
			if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EmberGameplayTags::GameplayEvent_Trace, nullptr, false, true))
			{
				GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnHitTarget);
				GameplayEventTask->ReadyForActivation();
			}

			if (UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("MeleeAttack"), AttackMontage, DefaultAttackRate, NAME_None, false, 1.f, 0.f, false))
			{
				PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageFinished);
				PlayMontageTask->ReadyForActivation();
			}

			if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EmberGameplayTags::GameplayEvent_Montage_End, nullptr, true, true))
			{
				GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnMontageEventTriggered);
				GameplayEventTask->ReadyForActivation();
			}
		}
	}
}

void UEmberGameplayAbility_MeleeAttack::OnHitTarget(FGameplayEventData Payload)
{
	if (HasAuthority(&CurrentActivationInfo) == false)
		return;

	AEquipmentBase* WeaponActor = const_cast<AEquipmentBase*>(Cast<AEquipmentBase>(Payload.Instigator));
	if (WeaponActor == nullptr)
		return;

	UEmberAbilitySystemComponent* SourceASC = GetEmberAbilitySystemComponentFromActorInfo();
	if (SourceASC == nullptr)
		return;

	if (SourceASC->FindAbilitySpecFromHandle(CurrentSpecHandle) == nullptr)
		return;

	float BaseDamage = GetEquipmentStatValue(EmberGameplayTags::ItemAttribute_BaseDamage, WeaponActor);

	FGameplayAbilityTargetDataHandle TargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(Payload.TargetData)));

	for (int32 i = 0; i < TargetDataHandle.Data.Num(); i++)
	{
		const TSharedPtr<FGameplayAbilityTargetData>& TargetData = TargetDataHandle.Data[i];

		FHitResult* HitResult = const_cast<FHitResult*>(TargetData->GetHitResult());
		if (HitResult == nullptr)
			continue;

		UGameplayStatics::ApplyPointDamage(
			HitResult->GetActor(),
			BaseDamage,
			HitResult->Location,
			*HitResult,
			GetEmberCharacterFromActorInfo()->GetInstigatorController(),
			WeaponActor,
			UDamageType::StaticClass()
		);
	}
}

void UEmberGameplayAbility_MeleeAttack::OnMontageEventTriggered(FGameplayEventData Payload)
{
	HandleMontageEvent(Payload);
}

void UEmberGameplayAbility_MeleeAttack::HandleMontageEvent(FGameplayEventData Payload)
{
	OnMontageFinished();
}

void UEmberGameplayAbility_MeleeAttack::OnMontageFinished()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

