#include "Interaction/Actors/Crafting/EternalFlame.h"

#include "AbilitySystemComponent.h"
#include "EmberPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameInfo/GameplayTags.h"
#include "Interaction/EmberBlueprintFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interaction/EmberTemperature.h"
#include "Notify/AnimNotifyState_SendGameplayEvent.h"
#include "Interaction/RespawnSubsystem.h"
#include "System/GameMode/C_EmberGameMode.h"

AEternalFlame::AEternalFlame()
{
	WarmingZone = CreateDefaultSubobject<USphereComponent>(TEXT("WarmingZone"));
	WarmingZone->SetupAttachment(GetRootComponent());
	WarmingZone->SetSphereRadius(800.0f);
	WarmingZone->SetCollisionProfileName(TEXT("Trigger"));
	WarmingZone->SetGenerateOverlapEvents(true);

	FlameEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlameEffect"));
	FlameEffect->SetupAttachment(GetRootComponent());
}

void AEternalFlame::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (WarmingZone)
		{
			WarmingZone->OnComponentBeginOverlap.AddDynamic(this, &AEternalFlame::OnWarmingZoneOverlapBegin);
			WarmingZone->OnComponentEndOverlap.AddDynamic(this, &AEternalFlame::OnWarmingZoneOverlapEnd);
		}
	}
}

void AEternalFlame::OnWarmingZoneOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor) return;

	if (IEmberTemperature* TemperatureHandler = Cast<IEmberTemperature>(OtherActor))
	{
		TemperatureHandler->Execute_ApplyWarmingEffect(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("EternalFlame: Applied warming effect to %s"), *OtherActor->GetName());
	}

	if (AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(OtherActor))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = EmberCharacter->GetAbilitySystemComponent())
		{
			FGameplayEventData Payload;
			Payload.EventTag = EmberGameplayTags::GameplayEvent_BodyTemperature_Change;
			Payload.Target = this;
			Payload.EventMagnitude = (int32)EBodyTemperatureChange::Increase;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
	}
}

void AEternalFlame::OnWarmingZoneOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority() || !OtherActor) return;
    
	if (IEmberTemperature* TemperatureHandler = Cast<IEmberTemperature>(OtherActor))
	{
		TemperatureHandler->Execute_RemoveWarmingEffect(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("EternalFlame: Removed warming effect from %s"), *OtherActor->GetName());
	}

	if (AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(OtherActor))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = EmberCharacter->GetAbilitySystemComponent())
		{
			FGameplayEventData Payload;
			Payload.EventTag = EmberGameplayTags::GameplayEvent_BodyTemperature_Change;
			Payload.Target = this;
			Payload.EventMagnitude = (int32)EBodyTemperatureChange::Decrease;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
	}
}

void AEternalFlame::OnInteractionSuccess(AActor* Interactor)
{
	Super::OnInteractionSuccess(Interactor);
	URespawnSubsystem* Subsystem = UEmberBlueprintFunctionLibrary::GetRespawnSubsystem();
	Subsystem->SetNewRespawnTransform(Interactor->GetTransform());
}
