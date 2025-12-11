#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindingEnemyTimer.generated.h"


UCLASS()
class EMBER_API UBTT_FindingEnemyTimer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTT_FindingEnemyTimer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void CheckDetection(bool IsDetect);
	void UpdateClosestActorTimer();

	FTimerHandle UpdateDistanceTimer;

	AActor* ClosestActor;
	UBehaviorTreeComponent* Owner;
};
