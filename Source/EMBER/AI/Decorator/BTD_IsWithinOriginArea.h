#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_IsWithinOriginArea.generated.h"


UCLASS(Blueprintable)
class EMBER_API UBTD_IsWithinOriginArea : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTD_IsWithinOriginArea();

	UPROPERTY(EditAnywhere, Category="AI")
	float MaxDistance;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;	
};
