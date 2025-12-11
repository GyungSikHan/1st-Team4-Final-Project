// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberAbilityTask_WaitForInteractableTraceHit.h"

#include "Interaction/EmberInteractable.h"

UEmberAbilityTask_WaitForInteractableTraceHit::UEmberAbilityTask_WaitForInteractableTraceHit(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TraceChannel = ECC_GameTraceChannel1;
	InteractionTraceRange = 100.f;
	InteractionTraceRate = 0.1f;
}

UEmberAbilityTask_WaitForInteractableTraceHit* UEmberAbilityTask_WaitForInteractableTraceHit::WaitForInteractableTraceHit(UGameplayAbility* OwningAbility, FEmberInteractionQuery InteractionQuery, ECollisionChannel TraceChannel, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionTraceRange, float InteractionTraceRate, bool bShowDebug)
{
	UEmberAbilityTask_WaitForInteractableTraceHit* Task = NewAbilityTask<UEmberAbilityTask_WaitForInteractableTraceHit>(OwningAbility);
	Task->InteractionTraceRange = InteractionTraceRange;
	Task->InteractionTraceRate = InteractionTraceRate;
	Task->StartLocation = StartLocation;
	Task->InteractionQuery = InteractionQuery;
	Task->TraceChannel = TraceChannel;
	Task->bShowDebug = bShowDebug;
	return Task;
}

void UEmberAbilityTask_WaitForInteractableTraceHit::Activate()
{
	Super::Activate();

	// Avatar가 생성되기까지 대기한다.
	SetWaitingOnAvatar();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TraceTimerHandle, this, &ThisClass::PerformTrace, InteractionTraceRate, true);
	}
}

void UEmberAbilityTask_WaitForInteractableTraceHit::OnDestroy(bool bInOwnerFinished)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TraceTimerHandle);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UEmberAbilityTask_WaitForInteractableTraceHit::PerformTrace()
{
	AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (AvatarActor == nullptr)
		return;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(EmberAbilityTask_WaitForInteractableTraceHit), false);
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;

	AimWithPlayerController(AvatarActor, Params, TraceStart, InteractionTraceRange, TraceEnd);

	FHitResult HitResult;
	LineTrace(TraceStart, TraceEnd, Params, HitResult);

	TArray<TScriptInterface<IEmberInteractable>> Interactables;
	TScriptInterface<IEmberInteractable> InteractableActor(HitResult.GetActor());
	if (InteractableActor)
	{
		Interactables.AddUnique(InteractableActor);
	}

	UpdateInteractionInfos(InteractionQuery, Interactables);

#if UE_EDITOR
	if (false)
	{
		FColor DebugColor = HitResult.bBlockingHit ? FColor::Red : FColor::Green;
		if (HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, DebugColor, false, InteractionTraceRate);
			DrawDebugSphere(GetWorld(), HitResult.Location, 5.f, 16, DebugColor, false, InteractionTraceRate);
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugColor, false, InteractionTraceRate);
		}
	}
#endif // ENABLE_DRAW_DEBUG
}

void UEmberAbilityTask_WaitForInteractableTraceHit::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch) const
{
	if (Ability == nullptr)
		return;

	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (PlayerController == nullptr)
		return;

	FVector CameraStart;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraStart, CameraRotation);

	const FVector CameraDirection = CameraRotation.Vector();

	// 카메라 방향의 Ray를 플레이어 위치 기준의 인터렉션 가능 범위(Sphere) 이내로 제한한다
	ClipCameraRayToAbilityRange(CameraStart, CameraDirection, TraceStart, MaxRange, OutTraceEnd);
}

bool UEmberAbilityTask_WaitForInteractableTraceHit::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& OutClippedPosition) const
{
	FVector CameraToCenter = AbilityCenter - CameraLocation;
	float DistanceCameraToDot = FVector::DotProduct(CameraToCenter, CameraDirection);
	if (DistanceCameraToDot >= 0)
	{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DistanceCameraToDot * DistanceCameraToDot);
		float RadiusSquared = (AbilityRange * AbilityRange);
		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceDotToSphere = FMath::Sqrt(RadiusSquared - DistanceSquared);
			float DistanceCameraToSphere = DistanceCameraToDot + DistanceDotToSphere;
			OutClippedPosition = CameraLocation + (DistanceCameraToSphere * CameraDirection);
			return true;
		}
	}
	return false;
}

void UEmberAbilityTask_WaitForInteractableTraceHit::LineTrace(const FVector& Start, const FVector& End, const FCollisionQueryParams& Params, FHitResult& OutHitResult) const
{
	TArray<FHitResult> HitResults;
	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, TraceChannel, Params);

	if (HitResults.Num() > 0)
	{
		OutHitResult = HitResults[0];
	}
	else
	{
		OutHitResult = FHitResult();
		OutHitResult.TraceStart = Start;
		OutHitResult.TraceEnd = End;
	}
}

void UEmberAbilityTask_WaitForInteractableTraceHit::UpdateInteractionInfos(const FEmberInteractionQuery& InteractQuery, const TArray<TScriptInterface<IEmberInteractable>>& Interactables)
{
	FEmberInteractionInfo NewInteractionInfo;

	for (const TScriptInterface<IEmberInteractable>& Interactable : Interactables)
	{
		if (Interactable->CanInteraction(AbilitySystemComponent.Get()))
		{
			NewInteractionInfo = Interactable->GatherPostInteractionInfo(InteractQuery, Interactable);
		}
	}

	bool bInfosChanged = (CurrentInteractionInfo != NewInteractionInfo);
	if (bInfosChanged)
	{
		HighlightInteractables(CurrentInteractionInfo, false);
		CurrentInteractionInfo = NewInteractionInfo;
		HighlightInteractables(CurrentInteractionInfo, true);
		
		InteractableChanged.Broadcast(CurrentInteractionInfo);
	}
}

void UEmberAbilityTask_WaitForInteractableTraceHit::HighlightInteractables(const FEmberInteractionInfo& InteractionInfo, bool bShouldHighlight)
{
	if (IEmberInteractable* Interactable = InteractionInfo.Interactable.GetInterface())
	{
		if (UMeshComponent* MeshComponent = Interactable->GetMeshComponent())
		{
			MeshComponent->SetRenderCustomDepth(bShouldHighlight);
		}
	}
}
