#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_DragonAttack.generated.h"

class ADragon;
class ACAIController;
class UAnimMontage;
class UDragonAnimInstance;

UCLASS()
class EMBER_API UBTT_DragonAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_DragonAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;

protected:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageTimeout();

private:
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* RangedAttackMontage;

	UPROPERTY(EditAnywhere)
	float LaunchZPower = 300.f;
	
	UPROPERTY()
	ADragon* Dragon;

	UPROPERTY()
	ACAIController* AIController;

	UPROPERTY()
	UDragonAnimInstance* DragonAnim;

	UPROPERTY()
	UBehaviorTreeComponent* BTComp;

	FTimerHandle MontageTimeoutHandle;
};
