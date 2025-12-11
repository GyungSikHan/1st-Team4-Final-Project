// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIComponent/CAIWeaponComponent.h"

#include "AIWeapon/CAI_Weapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCAIWeaponComponent::UCAIWeaponComponent()
{
}


// Called when the game starts
void UCAIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, L"%s", *GetOwner()->GetName());
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Owner is null");
		return;
	}
	int32 index{};
	for (int32 i = 0; i < WeaponDatas.Num(); i++)
	{
		if(WeaponDatas[i] != nullptr)
		{
			ACAI_Weapon* weapon = GetWorld()->SpawnActor<ACAI_Weapon>(WeaponDatas[i]);
			if (weapon != nullptr)
			{
				weapon->SpawnPlay(OwnerCharacter);
				Weapons.Add(weapon);
			}
		}
		else
			UE_LOG(LogTemp, Error, TEXT("weapon class is null"));
	}
}

void UCAIWeaponComponent::DoAction(int32 AttackIndex)
{
	if(GetDoAction() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Weapon is null");
		return;
	}
	GetDoAction()->DoAction(AttackIndex);
}

ACAI_Weapon* UCAIWeaponComponent::GetDoAction()
{
	if(Weapons[0] == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Weapon is null");
		return nullptr;
	}

	return Weapons[0];
}

void UCAIWeaponComponent::DestroyWeapon()
{
	for (ACAI_Weapon* weapon : Weapons)
	{
		weapon->Destroy();
	}
}

