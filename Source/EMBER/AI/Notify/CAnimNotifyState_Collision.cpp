// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Notify/CAnimNotifyState_Collision.h"

#include "AIComponent/CAIWeaponComponent.h"
#include "AIWeapon/CAI_Weapon.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if(MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, Mesh is null");
		return;
	}
	if(MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, Owner is null");
		return;
	}

	UCAIWeaponComponent* weapon = Cast<UCAIWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCAIWeaponComponent::StaticClass()));
	if(weapon == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, weapon comp is null");
		return;
	}
	if(weapon->GetDoAction() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, weapon is null");
		return;
	}

	weapon->GetDoAction()->OnCollision();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, Mesh is null");
		return;
	}
	if (MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, Owner is null");
		return;
	}

	UCAIWeaponComponent* weapon = Cast<UCAIWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCAIWeaponComponent::StaticClass()));
	if (weapon == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, weapon comp is null");
		return;
	}
	if (weapon->GetDoAction() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Collision, weapon is null");
		return;
	}

	weapon->GetDoAction()->OffCollision();
}