#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DragonMeteor.generated.h"

class ADragon;
class ACAIController;
class UDragonAnimInstance;

UCLASS()
class EMBER_API UBTT_DragonMeteor : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DragonMeteor();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;

	void FinishFromLanded();

protected:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
private:
	UPROPERTY(EditAnywhere, Category = "Meteor")
	UAnimMontage* MeteorMontage;

	UPROPERTY(EditAnywhere, Category = "Meteor")
	float LaunchZPower = 1000.f;

	UPROPERTY()
	ADragon* Dragon;

	UPROPERTY()
	ACAIController* AIController;

	UPROPERTY()
	UDragonAnimInstance* DragonAnim;

	UPROPERTY()
	UBehaviorTreeComponent* BTComp;
};
