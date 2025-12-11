
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_GriffonFly.generated.h"

class ABaseAI;
class ACAIController;



UCLASS()
class EMBER_API UBTT_GriffonFly : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTT_GriffonFly();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float FlyHeight;
	
	TObjectPtr<ACAIController> AIController;
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	TObjectPtr<ABaseAI> AI;
	
};
