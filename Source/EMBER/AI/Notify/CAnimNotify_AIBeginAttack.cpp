// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify_AIBeginAttack.h"

#include "C_CharacterMovementComponent.h"
#include "AIComponent/CAIWeaponComponent.h"
#include "AIWeapon/CAI_Weapon.h"

void UCAnimNotify_AIBeginAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AIBeginAttack Mesh Component is null");
		return;
	}
	if(MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AIBeginAttack Owner Actor is null");
		return;
	}

	UC_CharacterMovementComponent* Movement = Cast<UC_CharacterMovementComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_CharacterMovementComponent::StaticClass()));
	if(Movement == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AIBeginAttack Movement Component is null");
		return;
	}
	UCAIWeaponComponent* weapon = Cast<UCAIWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCAIWeaponComponent::StaticClass()));
	if (weapon == nullptr && weapon->GetDoAction() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AIBeginAttack weapon Component is null");
		return;
	}

	weapon->GetDoAction()->Begin_DoAction();
}

FString UCAnimNotify_AIBeginAttack::GetNotifyName_Implementation() const
{
	return "Begin AI Attack";
}
