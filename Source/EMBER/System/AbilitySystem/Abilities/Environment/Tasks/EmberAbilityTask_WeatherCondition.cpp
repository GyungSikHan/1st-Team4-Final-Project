// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberAbilityTask_WeatherCondition.h"

#include "EmberPlayerCharacter.h"
#include "StatusComponent.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"

UEmberAbilityTask_WeatherCondition::UEmberAbilityTask_WeatherCondition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

UEmberAbilityTask_WeatherCondition* UEmberAbilityTask_WeatherCondition::	WaitForDecreaseBodyTemperature(UGameplayAbility* OwningAbility, FEmberBodyTemperatureQuery BodyTemperatureQuery, float InDecreaseIntervalRate, float InDecreaseAmount)
{
	UEmberAbilityTask_WeatherCondition* Task = NewAbilityTask<UEmberAbilityTask_WeatherCondition>(OwningAbility);
	if (OwningAbility)
	{
		Task->TargetCharacter = Cast<AEmberPlayerCharacter>(OwningAbility->GetAvatarActorFromActorInfo());
	}
	Task->DecreaseIntervalRate = InDecreaseIntervalRate;
	Task->DecreaseAmount = InDecreaseAmount;
	
	return Task;
}

void UEmberAbilityTask_WeatherCondition::Activate()
{
	Super::Activate();

	// Avatar가 생성되기까지 대기한다.
	SetWaitingOnAvatar();
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DecreaseBodyTemperatureTimerHandle, this, &ThisClass::PerformTask, DecreaseIntervalRate, true);
	}
}

void UEmberAbilityTask_WeatherCondition::OnDestroy(bool bInOwnerFinished)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DecreaseBodyTemperatureTimerHandle);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UEmberAbilityTask_WeatherCondition::PerformTask()
{
	if (TargetCharacter == nullptr)
		return;

	UStatusComponent* const StatusComponent = TargetCharacter->GetStatusComponent();
	if (StatusComponent == nullptr)
		return;

	float CurrentTemperature = StatusComponent->GetTemperature();
	StatusComponent->AddTemperature(-DecreaseAmount);
}
