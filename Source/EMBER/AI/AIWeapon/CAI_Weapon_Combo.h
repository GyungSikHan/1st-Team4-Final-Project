// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CAI_Weapon.h"
#include "CAI_Weapon_Combo.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API ACAI_Weapon_Combo : public ACAI_Weapon
{
	GENERATED_BODY()
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo(){bEnable = false;}
public:
	ACAI_Weapon_Combo();

public:
	virtual void DoAction(int32 AttackIndex) override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

private:
	bool bEnable;
	bool bExist;
};
