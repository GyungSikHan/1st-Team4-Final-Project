#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/CBehaviorTreeComponent.h"
#include "DragonBehaviorTreeComponent.generated.h"

// UENUM(BlueprintType)
// enum class EDragonAttackState : uint8
// {
// 	None	UMETA(DisplayName = "None"),
// 	Normal	UMETA(DisplayName = "Normal"),
// 	Combo	UMETA(DisplayName = "Combo"),
// 	Spit	UMETA(DisplayName = "Spit"),
// 	Breath	UMETA(DisplayName = "Breath"),
// };
UCLASS()
class EMBER_API UDragonBehaviorTreeComponent : public UCBehaviorTreeComponent
{
	GENERATED_BODY()

public:
	// void SetAttackState(EDragonAttackState InState);
	// EDragonAttackState GetAttackState() const;

	// bool IsNormalAttack() const;
	// bool IsComboAttack() const;
	// bool IsSpitAttack() const;
	// bool IsBreathAttack() const;
	//
	// void SetNormalAttackMode();
	// void SetComboAttackMode();
	// void SetSpitAttackMode();
	// void SetBreathAttackMode();

	//void SetBlackboard_Enum(FName Keyname, EDragonAttackState Value);

private:
	// UPROPERTY(EditAnywhere, Category = "Key")
	// FName AttackStateKey = "AttackState";
};
