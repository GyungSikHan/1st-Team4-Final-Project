// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility.h"

#include "EmberLocalPlayer.h"
#include "Player/EmberPlayerCharacter.h"
#include "Player/EmberPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameInfo/GameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/AbilitySystem/EmberAbilitySystemComponent.h"
#include "System/AbilitySystem/Costs/EmberAbilityCost.h"

UEmberGameplayAbility::UEmberGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	
	ActivationPolicy = EEmberAbilityActivationPolicy::OnInputTriggered;
}

void UEmberGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == EEmberAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

UEmberAbilitySystemComponent* UEmberGameplayAbility::GetEmberAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UEmberAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

AEmberPlayerCharacter* UEmberGameplayAbility::GetEmberCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AEmberPlayerCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

AEmberPlayerController* UEmberGameplayAbility::GetEmberPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AEmberPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* UEmberGameplayAbility::GetControllerFromActorInfo() const
{
	return (CurrentActorInfo ? CurrentActorInfo->PlayerController.Get() : nullptr);
}

UEmberLocalPlayer* UEmberGameplayAbility::GetEmberLocalPlayerFromActorInfo() const
{
	if (APlayerController* Controller = CurrentActorInfo->PlayerController.Get())
	{
		return Cast<UEmberLocalPlayer>(Controller->Player);
	}

	return nullptr;
}

bool UEmberGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

bool UEmberGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bBlocked = false;
	bool bMissing = false;

	UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
	const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;

	// Check if any of this ability's tags are currently blocked
	if (AbilitySystemComponent.AreAbilityTagsBlocked(AbilityTags))
	{
		bBlocked = true;
	}

	const UEmberAbilitySystemComponent* EmberASC = Cast<UEmberAbilitySystemComponent>(&AbilitySystemComponent);
	static FGameplayTagContainer AllRequiredTags;
	static FGameplayTagContainer AllBlockedTags;

	AllRequiredTags = ActivationRequiredTags;
	AllBlockedTags = ActivationBlockedTags;

	if (EmberASC)
	{
		EmberASC->GetAdditionalActivationTagRequirements(AbilityTags, AllRequiredTags, AllBlockedTags);
	}

	if (AllBlockedTags.Num() || AllRequiredTags.Num())
	{
		static FGameplayTagContainer AbilitySystemComponentTags;
		
		AbilitySystemComponentTags.Reset();
		AbilitySystemComponent.GetOwnedGameplayTags(AbilitySystemComponentTags);

		if (AbilitySystemComponentTags.HasAny(AllBlockedTags))
		{
			bBlocked = true;
		}

		if (!AbilitySystemComponentTags.HasAll(AllRequiredTags))
		{
			bMissing = true;
		}
	}

	if (SourceTags != nullptr)
	{
		if (SourceBlockedTags.Num() || SourceRequiredTags.Num())
		{
			if (SourceTags->HasAny(SourceBlockedTags))
			{
				bBlocked = true;
			}

			if (!SourceTags->HasAll(SourceRequiredTags))
			{
				bMissing = true;
			}
		}
	}

	if (TargetTags != nullptr)
	{
		if (TargetBlockedTags.Num() || TargetRequiredTags.Num())
		{
			if (TargetTags->HasAny(TargetBlockedTags))
			{
				bBlocked = true;
			}

			if (!TargetTags->HasAll(TargetRequiredTags))
			{
				bMissing = true;
			}
		}
	}
	
	if (bBlocked)
	{
		if (OptionalRelevantTags && BlockedTag.IsValid())
		{
			OptionalRelevantTags->AddTag(BlockedTag);
		}
		return false;
	}
	if (bMissing)
	{
		if (OptionalRelevantTags && MissingTag.IsValid())
		{
			OptionalRelevantTags->AddTag(MissingTag);
		}
		return false;
	}

	return true;
}

bool UEmberGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) == false || ActorInfo == nullptr)
	{
		return false;
	}

	for (const TObjectPtr<UEmberAbilityCost>& AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			if (false == AdditionalCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
			{
				return false;
			}
		}
	}

	return true;
}

void UEmberGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	for (const TObjectPtr<UEmberAbilityCost>& AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			AdditionalCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
		}
	}
}

void UEmberGameplayAbility::GetMovementDirection(EEmberDirection& OutDirection, FVector& OutMovementVector) const
{
	FVector FacingVector = FVector::ZeroVector;
	
	if (AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo())
	{
		FacingVector = EmberCharacter->GetActorForwardVector();
		OutMovementVector = EmberCharacter->GetLastMovementInputVector();
	}

	const FRotator& FacingRotator = UKismetMathLibrary::Conv_VectorToRotator(FacingVector);
	const FRotator& MovementRotator = UKismetMathLibrary::Conv_VectorToRotator(OutMovementVector);

	const FRotator& DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotator, FacingRotator);
	float YawAbs = FMath::Abs(DeltaRotator.Yaw);

	if (OutMovementVector.IsNearlyZero())
	{
		OutDirection = EEmberDirection::None;
	}
	else
	{
		if (YawAbs < 60.f)
		{
			OutDirection = EEmberDirection::Forward;
		}
		else if (YawAbs > 120.f)
		{
			OutDirection = EEmberDirection::Backward;
		}
		else if (DeltaRotator.Yaw < 0.f)
		{
			OutDirection = EEmberDirection::Left;
		}
		else
		{
			OutDirection = EEmberDirection::Right;
		}
	}
}
