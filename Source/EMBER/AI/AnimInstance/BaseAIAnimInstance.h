#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAIAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EAnimActionType : uint8
{
	Idle,
	Run,
	Eat,
	Death,
	
	AttackNormal,
	AttackJump,
	AttackRun,
	
	HitFront,
	HitLeft,
	HitRight,

	MAX
};

UCLASS()
class EMBER_API UBaseAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Movement")
	float CurrentSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement")
	float CurrentDirection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim")
	EAnimActionType DesiredActionType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Anim")
	EAnimActionType FallbackActionType;
	
	virtual void PlayMontage();
	virtual auto PlayStateMontage(EAnimActionType Action) -> void; //AnimalState에 따라 자동으로 몽타주 재생
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// EAnimalState AnimalState;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim")
	TMap<EAnimActionType, FName> AnimSectionMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim")
	UAnimMontage* IdleMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim")
	UAnimMontage* HitMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Anim")
	UAnimMontage* DeathMontage;

	virtual UAnimMontage* GetMontageToPlay(EAnimActionType ActionType) const;
	//virtual UAnimMontage* GetMontageToPlay();
};