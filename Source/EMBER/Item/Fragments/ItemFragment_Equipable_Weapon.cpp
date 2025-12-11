// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFragment_Equipable_Weapon.h"

UItemFragment_Equipable_Weapon::UItemFragment_Equipable_Weapon(const FObjectInitializer& FObjectInitializer) : Super(FObjectInitializer)
{
	EquipmentType = EEquipmentType::Weapon;
	AttackInfo.PlayRate = 1.0f;
}

void UItemFragment_Equipable_Weapon::OnInstanceCreated(UItemInstance* ItemInstance) const
{
	Super::OnInstanceCreated(ItemInstance);
	
	AddStatTagStack(ItemInstance, RarityStatRangeSets);
}

//void UItemFragment_Equipable_Weapon::IncrementMontageIndex()
//{
//	if (AttackInfo.Montages.Num() > 0)
//	{
//		int32 Index = AttackInfo.MontageIndex+1;
//		AttackInfo.MontageIndex = Index % AttackInfo.Montages.Num();
//	}
//}

FAttackData UItemFragment_Equipable_Weapon::GetAttackInfo() const
{
	return AttackInfo;
}
