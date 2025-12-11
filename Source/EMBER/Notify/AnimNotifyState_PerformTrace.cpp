// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_PerformTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EmberPlayerCharacter.h"
#include "KismetTraceUtils.h"
#include "Components/BoxComponent.h"
#include "Equipment/EquipmentBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Managers/EquipmentManagerComponent.h"

class AEmberPlayerCharacter;

UAnimNotifyState_PerformTrace::UAnimNotifyState_PerformTrace(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = false;
#endif
}

void UAnimNotifyState_PerformTrace::NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComponent, Animation, TotalDuration, EventReference);

	if (MeshComponent->GetOwnerRole() != ExecuteNetRole)
		return;

	if (AEmberPlayerCharacter* Character = Cast<AEmberPlayerCharacter>(MeshComponent->GetOwner()))
	{
		if (UEquipmentManagerComponent* EquipmentManager = Character->FindComponentByClass<UEquipmentManagerComponent>())
		{
			WeaponActor = EquipmentManager->GetEquippedActor(WeaponHandType);
			if (WeaponActor.IsValid() == false)
				return;
			
			PreviousTraceTransform = WeaponActor->MeshComponent->GetComponentTransform();
			PreviousDebugTransform = WeaponActor->TraceDebugCollision->GetComponentTransform();
			PreviousSocketTransform = WeaponActor->MeshComponent->GetSocketTransform(TraceParams.TraceSocketName);
		}
	}

	HitActors.Empty();
}

void UAnimNotifyState_PerformTrace::NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComponent, Animation, FrameDeltaTime, EventReference);

	if (MeshComponent->GetOwnerRole() != ExecuteNetRole)
		return;
	
	if (WeaponActor.IsValid() == false)
		return;
	
	PerformTrace(MeshComponent);
}

void UAnimNotifyState_PerformTrace::NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComponent, Animation, EventReference);

	if (MeshComponent->GetOwnerRole() != ExecuteNetRole)
		return;
	
	if (WeaponActor.IsValid() == false)
		return;
	
	PerformTrace(MeshComponent);
}

void UAnimNotifyState_PerformTrace::PerformTrace(USkeletalMeshComponent* MeshComponent)
{
	if (WeaponActor.IsValid() == false)
		return;
	
	FTransform CurrentSocketTransform = WeaponActor->MeshComponent->GetSocketTransform(TraceParams.TraceSocketName);
	float Distance = (PreviousSocketTransform.GetLocation() - CurrentSocketTransform.GetLocation()).Length();

	int SubStepCount = FMath::CeilToInt(Distance / TraceParams.TargetDistance);
	if (SubStepCount <= 0)
		return;

	float SubstepRatio = 1.f / SubStepCount;
	
	const FTransform& CurrentTraceTransform = WeaponActor->MeshComponent->GetComponentTransform();
	const FTransform& CurrentDebugTransform = WeaponActor->TraceDebugCollision->GetComponentTransform();

	TArray<FHitResult> FinalHitResults;
	
	for (int32 i = 0; i < SubStepCount; i++)
	{
		FTransform StartTraceTransform = UKismetMathLibrary::TLerp(PreviousTraceTransform, CurrentTraceTransform, SubstepRatio * i, ELerpInterpolationMode::DualQuatInterp);
		FTransform EndTraceTransform = UKismetMathLibrary::TLerp(PreviousTraceTransform, CurrentTraceTransform, SubstepRatio * (i + 1), ELerpInterpolationMode::DualQuatInterp);
		FTransform AverageTraceTransform = UKismetMathLibrary::TLerp(StartTraceTransform, EndTraceTransform, 0.5f, ELerpInterpolationMode::DualQuatInterp);

		FComponentQueryParams Params = FComponentQueryParams::DefaultComponentQueryParams;
		Params.bReturnPhysicalMaterial = true;

		TArray<AActor*> IgnoredActors = { WeaponActor.Get(), WeaponActor->GetOwner() };
		Params.AddIgnoredActors(IgnoredActors);

		TArray<FHitResult> HitResults;
		MeshComponent->GetWorld()->ComponentSweepMulti(HitResults, WeaponActor->MeshComponent, StartTraceTransform.GetLocation(), EndTraceTransform.GetLocation(), AverageTraceTransform.GetRotation(), Params);

		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActors.Contains(HitActor) == false)
			{
				HitActors.Add(HitActor);
				FinalHitResults.Add(HitResult);
			}
		}

#if UE_EDITOR
		if (GIsEditor)
		{
			if (true && TraceDebugParams.bDrawDebugShape)
			{
				FColor Color = (HitResults.Num() > 0) ? TraceDebugParams.HitColor : TraceDebugParams.TraceColor;
				
				FTransform StartDebugTransform = UKismetMathLibrary::TLerp(PreviousDebugTransform, CurrentDebugTransform, SubstepRatio * i, ELerpInterpolationMode::DualQuatInterp);
				FTransform EndDebugTransform = UKismetMathLibrary::TLerp(PreviousDebugTransform, CurrentDebugTransform, SubstepRatio * (i + 1), ELerpInterpolationMode::DualQuatInterp);
				FTransform AverageDebugTransform = UKismetMathLibrary::TLerp(StartDebugTransform, EndDebugTransform, 0.5f, ELerpInterpolationMode::DualQuatInterp);
				
				DrawDebugSweptBox(MeshComponent->GetWorld(), StartDebugTransform.GetLocation(), EndDebugTransform.GetLocation(), AverageDebugTransform.GetRotation().Rotator(), WeaponActor->TraceDebugCollision->GetScaledBoxExtent(), Color, false, 2.f);
			}
		}
#endif
	}

	PreviousTraceTransform = CurrentTraceTransform;
	PreviousDebugTransform = CurrentDebugTransform;
	PreviousSocketTransform = CurrentSocketTransform;

	if (FinalHitResults.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetDataHandle;
		
		for (const FHitResult& HitResult : FinalHitResults)
		{
			FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = HitResult;
			TargetDataHandle.Add(NewTargetData);
		}
		
		FGameplayEventData EventData;
		EventData.TargetData = TargetDataHandle;
		EventData.Instigator = WeaponActor.Get();

		if (EventTag.IsValid())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComponent->GetOwner(), EventTag, EventData);
		}
	}
}
