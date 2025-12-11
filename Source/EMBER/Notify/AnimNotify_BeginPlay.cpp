// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_BeginPlay.h"

#include "C_CameraComponent.h"
#include "C_CharacterMovementComponent.h"
#include "GameData.h"
#include "Managers/EquipmentManagerComponent.h"

void UAnimNotify_BeginPlay::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Mesh Component is null");
		return;
	}
	if(MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Owner Actor is null");
		return;
	}

	UC_CharacterMovementComponent* Movement = Cast<UC_CharacterMovementComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_CharacterMovementComponent::StaticClass()));
	if(Movement == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Movement Component is null");
		return;
	}
	UEquipmentManagerComponent* Equipment = Cast<UEquipmentManagerComponent>(MeshComp->GetOwner()->GetComponentByClass(UEquipmentManagerComponent::StaticClass()));
	if (Equipment == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Equipment Component is null");
		return;
	}
	UC_CameraComponent* Camera = Cast<UC_CameraComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_CameraComponent::StaticClass()));
	if(Camera == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Camera Component is null");
		return;
	}

	if (Equipment->GetAttackInfo().bCanMove == false)
		Movement->DisableMove();
	if (Equipment->GetAttackInfo().bCanFixedCamera == true)
		Camera->EnableFixedCamera();
}

FString UAnimNotify_BeginPlay::GetNotifyName_Implementation() const
{
	return "Begin Attack";
}
