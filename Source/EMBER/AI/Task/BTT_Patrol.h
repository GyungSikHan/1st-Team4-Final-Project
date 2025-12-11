#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h" 
#include "BTT_Patrol.generated.h"

UCLASS()
class EMBER_API UBTT_Patrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Patrol();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
	FAIRequestID PatrolRequestID;
	UBlackboardComponent* BlackboardComp;
	UBehaviorTreeComponent* OwnerCompRef;
	
	FDelegateHandle MoveCompleteHandle;
};
