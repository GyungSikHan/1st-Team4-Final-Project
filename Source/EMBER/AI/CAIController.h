// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_StateComponent.h"
#include "CAIController.generated.h"

class ABaseAI;
struct FAIStimulus;
class UAISenseConfig_Sight;
class AHumanAIBase;
class UCBehaviorTreeComponent;

UENUM(BlueprintType)
enum class
	EAnimalType : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Defensive UMETA(DisplayName = "Defensive"),
	Aggressive UMETA(DisplayName = "Aggressive"),
	Crow UMETA(DisplayName = "Crow"),
	Griffon UMETA(DisplayName = "Griffon"),
	Dragon UMETA(DisplayName = "Dragon"),
	Human UMETA(DisplayName = "Hunam")
};

UCLASS()
class EMBER_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UAIPerceptionComponent> GetPercption() { return Perception; }

public:
	ACAIController();
	TArray<AActor*> Actors;

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
	virtual void BeginPlay() override;

	// 	UFUNCTION()
	// 	void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Target, FAIStimulus Stimulus);


	//UFUNCTION()
	//virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> Perception;
	TObjectPtr<UC_StateComponent> AIState;
	TObjectPtr<ABaseAI> AI;
	TObjectPtr<UCBehaviorTreeComponent> Behavior;
	TObjectPtr<UAISenseConfig_Sight> Sight;
};
