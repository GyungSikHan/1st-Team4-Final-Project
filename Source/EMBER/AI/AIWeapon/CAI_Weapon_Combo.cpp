// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIWeapon/CAI_Weapon_Combo.h"

#include "CAIController.h"
#include "C_StateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

ACAI_Weapon_Combo::ACAI_Weapon_Combo()
{
}

void ACAI_Weapon_Combo::DoAction(int32 AttackIndex)
{
	// UE_LOG(LogTemp, Error, L"Action %d", AttackDatas.Num());
	if(AttackDatas.Num() < 1)
		return;
	if(AttackDatas.Num() < AttackIndex)
		return;
	if(bEnable == true)
	{
		bEnable = false;
		bExist = true;
		return;
	}

	//TODOS combatMode ����
	if (State.Get()->IsIdleMode() == false )
		if(State.Get()->IsCombatMode() == false)
			if(State.Get()->IsDetectMode() == false)
			{
				UE_LOG(LogTemp, Error, L"Is not Idle State");
				return;
			}

	Super::DoAction(AttackIndex);
	CurrAttackIndex = AttackIndex;
	AttackDatas[AttackIndex].DoAction(OwnerCharacter);
}

void ACAI_Weapon_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	/*ACAIController* AIcontroller = OwnerCharacter->GetController<ACAIController>();
	if(AIcontroller != nullptr)
	{
		float random = (float)UKismetMathLibrary::RandomFloatInRange(0, 2);
		UE_LOG(LogTemp, Warning, L"%f", random);
		if (random != 2)
			bExist = true;
	}
	if(bExist == false)
		return;

	bExist = false;

	Index++;
	if (Index >= AttackDatas.Num())
		return;
	AttackDatas[Index].DoAction(OwnerCharacter);*/
}

void ACAI_Weapon_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}
