#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_RotateToActor.generated.h"

class ABaseAI;
class UBlackboardComponent;

UCLASS()
class EMBER_API UBTT_RotateToActor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UBTT_RotateToActor();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere)
	UAnimMontage* TurnLeftMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* TurnRightMontage;

	ABaseAI* AI;
	AActor* Target;
	UBlackboardComponent* BlackboardComp;
	
	FRotator TargetRotation;
	FRotator CurrentRotation;
};
