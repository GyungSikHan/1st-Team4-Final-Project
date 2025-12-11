
#pragma once

#include "CoreMinimal.h"
#include "CAIController.h"
#include "AI/AnimInstance/BaseAIAnimInstance.h"
#include "Base/BaseAI.h"
#include "Griffon_AnimInstance.generated.h"


UCLASS()
class EMBER_API UGriffon_AnimInstance : public UBaseAIAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeBeginPlay() override;
	TObjectPtr<UBlackboardComponent> Blackboard;
	TObjectPtr<ACAIController> Controller;

	void PlayLandingAnimation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	bool bIsAirborne;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	float CurrentFlySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	UAnimMontage* LandingMontage;
};
