#pragma once

#include "CoreMinimal.h"
#include "AI/AnimInstance/BaseAIAnimInstance.h"
#include "Mammoth_AnimInstance.generated.h"


UCLASS()
class EMBER_API UMammoth_AnimInstance : public UBaseAIAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AnimNotify_Idle();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_Run();
	UFUNCTION(BlueprintCallable)
	void AnimNotify_Eat();
};
