#pragma once

#include "CoreMinimal.h"
#include "AI/AnimInstance/BaseAIAnimInstance.h"
#include "DragonAnimInstance.generated.h"

class ADragon;
class UNiagaraSystem;

UCLASS()
class EMBER_API UDragonAnimInstance : public UBaseAIAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

protected:
	UFUNCTION()
	void AnimNotify_SpawnSpit();

	UFUNCTION()
	void AnimNotify_SpawnEffect();

	UFUNCTION()
	void AnimNotify_SpawnBreath();

	UFUNCTION()
	void AnimNotify_BreathEnd();

	UFUNCTION()
	void AnimNotify_Fly();	

	UFUNCTION()
	void AnimNotify_Land();

	UFUNCTION()
	void AnimNotify_Glide();

private:
	TObjectPtr<ADragon> Dragon;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* SpawnNiagara;
};
