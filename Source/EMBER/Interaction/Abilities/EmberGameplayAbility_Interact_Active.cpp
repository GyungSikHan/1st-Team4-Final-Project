#include "EmberGameplayAbility_Interact_Active.h"

#include "AbilitySystemComponent.h"
#include "EmberPlayerCharacter.h"
#include "GameInfo/GameplayTags.h"
#include "Interaction/Actors/EmberWorldInteractable.h"

UEmberGameplayAbility_Interact_Active::UEmberGameplayAbility_Interact_Active(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::Manual;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// 클라이언트가 능력을 취소 요청하면 서버가 이를 수용
	bServerRespectsRemoteAbilityCancellation = true;
	// 클라이언트는 EndAbility()를 호출해도 요청만 할 수 있고, 실제로 종료하는 권한은 서버에 있음.
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;

	AbilityTags.AddTag(EmberGameplayTags::Ability_Interact_Active);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Interact);

	// RF_ClassDefaultObject
	// 클래스가 처음 로드될 때 해당 플래그가 한 번 설정된다.
	// 플래그 체크를 통해 아래 로직이 한 번만 설정되도록 보장한다.
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::Ability_Interact_Active;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UEmberGameplayAbility_Interact_Active::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (InitializeAbility(const_cast<AActor*>(TriggerEventData->Target.Get())) == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	TriggerInteraction();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

bool UEmberGameplayAbility_Interact_Active::TriggerInteraction()
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG 3] TriggerInteraction function has started!"));

	bool bCanActivate = false;
	bool bTriggerSuccessful = false;

	FGameplayEventData Payload;
	Payload.EventTag = EmberGameplayTags::Ability_Interact;
	Payload.Instigator = GetAvatarActorFromActorInfo();
	Payload.Target = InteractableActor;
    
	if (UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo())
	{
		FString AbilityClassName = IsValid(InteractionInfo.AbilityToGrant) ? InteractionInfo.AbilityToGrant->GetName() : TEXT("None");
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG 3] AbilityToGrant is: %s"), *AbilityClassName);

		if (FGameplayAbilitySpec* AbilitySpec = AbilitySystem->FindAbilitySpecFromClass(InteractionInfo.AbilityToGrant))
		{
			bCanActivate = AbilitySpec->Ability->CanActivateAbility(AbilitySpec->Handle, AbilitySystem->AbilityActorInfo.Get());
          
			UE_LOG(LogTemp, Warning, TEXT("[DEBUG 3] CanActivateAbility returned: %s"), bCanActivate ? TEXT("true") : TEXT("false"));

			bTriggerSuccessful = AbilitySystem->TriggerAbilityFromGameplayEvent(
			   AbilitySpec->Handle,
			   AbilitySystem->AbilityActorInfo.Get(),
			   EmberGameplayTags::Ability_Interact,
			   &Payload,
			   *AbilitySystem
			);

			UE_LOG(LogTemp, Warning, TEXT("[DEBUG 3] TriggerAbilityFromGameplayEvent returned: %s"), bTriggerSuccessful ? TEXT("true") : TEXT("false"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[DEBUG 3] FAILED: FindAbilitySpecFromClass could not find the ability. Is the ability granted to the player?"));
		}
	}

	return bCanActivate || bTriggerSuccessful;
}
