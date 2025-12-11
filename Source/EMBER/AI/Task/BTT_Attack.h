#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Attack.generated.h"


UCLASS()
class EMBER_API UBTT_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	int32 AttackIndex;

	//protected:
//	UFUNCTION()
//	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
//	
//	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
//	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
//private:
//	UPROPERTY()
//	UAnimInstance* CachedAnimInstance;
//
//	bool bIsMontagePlaying;
};
