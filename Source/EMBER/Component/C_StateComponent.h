#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFlag.h"
#include "C_StateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API UC_StateComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UC_StateComponent();
	EStateType CurrentStateType;
	EStateType PrevStateType;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:	
	bool IsIdleMode() const;
	bool IsRunMode() const;
	bool IsAnimMode() const;
	bool IsEquipMode() const;
	bool IsActionMode() const;
	bool IsCombatMode() const;
	bool IsHittdMode() const;
	bool IsDeadMode() const;
	bool IsDetectMode() const;
	void SetIdleMode();
	void SetRunMode();
	void SetAnimMode();
	void SetEquipMode();
	void SetActionMode();
	void SetCombatMode();
	void SetHittdMode();
	void SetDeadMode();
	void SetDetectMode();


private:
	void ChangeType(EStateType InType);
public:
	FStateTypeChanged OnStateTypeChanged;
};
