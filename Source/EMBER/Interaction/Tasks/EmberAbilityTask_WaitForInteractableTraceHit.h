// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "Interaction/EmberInteractionInfo.h"
#include "Interaction/EmberInteractionQuery.h"
#include "EmberAbilityTask_WaitForInteractableTraceHit.generated.h"

class IEmberInteractable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableChanged, const FEmberInteractionInfo&, InteractableInfo);


UCLASS()
class EMBER_API UEmberAbilityTask_WaitForInteractableTraceHit : public UAbilityTask
{
	GENERATED_BODY()

public:
	UEmberAbilityTask_WaitForInteractableTraceHit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UEmberAbilityTask_WaitForInteractableTraceHit* WaitForInteractableTraceHit(UGameplayAbility* OwningAbility, FEmberInteractionQuery InteractionQuery, ECollisionChannel TraceChannel, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionTraceRange = 100.f, float InteractionTraceRate = 0.1f, bool bShowDebug = false);

protected:
	//~UAbilityTask Overrides
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~End of AbilityTask Overrides

private:
	void PerformTrace();

	void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch = false) const;
	bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& OutClippedPosition) const;

	void LineTrace(const FVector& Start, const FVector& End, const FCollisionQueryParams& Params, FHitResult& OutHitResult) const;

	void UpdateInteractionInfos(const FEmberInteractionQuery& InteractQuery, const TArray<TScriptInterface<IEmberInteractable>>& Interactables);
	void HighlightInteractables(const FEmberInteractionInfo& InteractionInfo, bool bShouldHighlight);

public:
	UPROPERTY(BlueprintAssignable)
	FOnInteractableChanged InteractableChanged;
	
private:
	UPROPERTY()
	FEmberInteractionQuery InteractionQuery;

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;

/* Trace */
private:
	ECollisionChannel TraceChannel;
	float InteractionTraceRange;
	float InteractionTraceRate;
	FTimerHandle TraceTimerHandle;
	bool bShowDebug;

private:
	FEmberInteractionInfo CurrentInteractionInfo;
};
