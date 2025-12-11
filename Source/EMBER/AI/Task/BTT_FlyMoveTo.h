#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FlyMoveTo.generated.h"

class ABaseAI;


UCLASS()
class EMBER_API UBTT_FlyMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
UBTT_FlyMoveTo();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool IsNearGround();
	bool IsNearTargetLocation();
	
	ABaseAI* BaseAI;
	AActor* TargetActor;
	FVector TargetLocation = FVector::ZeroVector;
	FVector CurrentLocation;
	FVector Direction;
	UBlackboardComponent* BlackboardComp;
	
	UPROPERTY(EditAnywhere)
	float AcceptableRadius = 200.f;
	
	// UPROPERTY(EditAnywhere, Category = "Blackboard")
	// FBlackboardKeySelector TargetActorKey;
	
	// UPROPERTY(EditAnywhere)
	// AActor* TargetActor;
	// UPROPERTY(EditAnywhere)
	// FVector TargetLocation;
};
