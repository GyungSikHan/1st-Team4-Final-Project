// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFlag.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PerformTrace.generated.h"

class AEquipmentBase;

USTRUCT(BlueprintType)
struct FTraceParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetDistance = 20.f;

	UPROPERTY(EditAnywhere)
	FName TraceSocketName = "TraceSocket";
};

USTRUCT(BlueprintType)
struct FTraceDebugParams
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bDrawDebugShape = false;
	
	UPROPERTY(EditAnywhere)
	FColor TraceColor = FColor::Red;

	UPROPERTY(EditAnywhere)
	FColor HitColor = FColor::Green;
};

UCLASS()
class EMBER_API UAnimNotifyState_PerformTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_PerformTrace(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UAnimNotifyState Overrides
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//~End of UAnimNotifyState Overrides

private:
	void PerformTrace(USkeletalMeshComponent* MeshComponent);


public:
	UPROPERTY(EditAnywhere)
	EWeaponHandType WeaponHandType = EWeaponHandType::RightHand;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ENetRole> ExecuteNetRole = ROLE_Authority;

	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;
	
	UPROPERTY(EditAnywhere)
	FTraceParams TraceParams;

	UPROPERTY(EditAnywhere)
	FTraceDebugParams TraceDebugParams;
	
private:
	UPROPERTY()
	TWeakObjectPtr<AEquipmentBase> WeaponActor;
	
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> HitActors;

private:
	FTransform PreviousTraceTransform;
	FTransform PreviousDebugTransform;
	FTransform PreviousSocketTransform;
};
