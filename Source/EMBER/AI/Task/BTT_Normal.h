#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Normal.generated.h"


UCLASS()
class EMBER_API UBTT_Normal : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Normal();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};