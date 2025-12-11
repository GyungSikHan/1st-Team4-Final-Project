#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ResetValue.generated.h"


UCLASS()
class EMBER_API UBTT_ResetValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_ResetValue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
