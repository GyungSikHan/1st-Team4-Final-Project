#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFlag.h"
#include "InputActionValue.h"
#include "C_CharacterMovementComponent.generated.h"

// 델리게이트 선언: FVector2D를 파라미터로 받는 동적 멀티캐스트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveDelegate, FVector2D, MoveVector);

UCLASS()
class EMBER_API UC_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, category = "Move")
	bool bCanMove = true;
	UPROPERTY(EditAnywhere, category = "Move")
	bool bIsDash;
	//UPROPERTY(EditAnywhere, category = "State")
	class UC_StateComponent* State;
	UPROPERTY(EditAnywhere, category = "Base")
	class ACharacter* OwnerCharacter;
	UPROPERTY(EditAnywhere, category = "Move")
	float Speed[(int32)ESpeedType::Max]{200,400,600};
	UPROPERTY(EditAnywhere, category = "Move")
	float FlySpeed[(int32)EFlySpeedType::Max]{400,600,900};
	
protected:	//	재정의
	virtual void BeginPlay() override;
	//virtual float GetMaxSpeed() const override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:	//	추가
	UFUNCTION(BlueprintCallable)
	bool GetCanMove() const;
	UFUNCTION(BlueprintCallable)
	bool GetCanDash() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentSpeed() const;
	UFUNCTION(BlueprintCallable)
	void EnableMove();
	UFUNCTION(BlueprintCallable)
	void DisableMove();
	UFUNCTION(BlueprintCallable)
	void OnMove(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void OnJump();

public:
	void OnSprint();
	void OnRun();
	void OnWalk();

public:
	void SetSpeed(ESpeedType SpeedType);
	void SetFlySpeed(EFlySpeedType FlySpeedType);
	void SetWalkSpeed(int32 InSpeed); 
	void SetRunSpeed(int32 InSpeed); 
	void SetSprintSpeed(int32 InSpeed);
public: 
	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnMoveDelegate OnMoveDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	float LeftRightMovePercent = 0.7f;
	
	UPROPERTY(EditDefaultsOnly)
	float BackwardMovePercent = 0.7f;
	
private:
	ESpeedType CurrentSpeedType;
};
