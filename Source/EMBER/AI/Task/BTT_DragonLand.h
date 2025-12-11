#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DragonLand.generated.h"

class ADragon;
class ACAIController;
class UBehaviorTreeComponent;
class UAnimMontage;

UCLASS(Blueprintable)
class EMBER_API UBTT_DragonLand : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DragonLand();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, Category = "Fly")
	UAnimMontage* LandMontage;

private:
	UPROPERTY()
	ADragon* Dragon;

	UPROPERTY()
	ACAIController* AIController;

	UPROPERTY()
	UBehaviorTreeComponent* BTComp;

	bool bMontagePlaying;
};
