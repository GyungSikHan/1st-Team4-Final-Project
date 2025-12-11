// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotify_EndPlay.h"

#include "C_CameraComponent.h"
#include "C_CharacterMovementComponent.h"
#include "C_StateComponent.h"

void UAnimNotify_EndPlay::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Mesh Component is null");
		return;
	}
	if (MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Owner Actor is null");
		return;
	}

	UC_CharacterMovementComponent* Movement = Cast<UC_CharacterMovementComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_CharacterMovementComponent::StaticClass()));
	if (Movement == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Movement Component is null");
		return;
	}
	UC_CameraComponent* Camera = Cast<UC_CameraComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_CameraComponent::StaticClass()));
	if (Camera == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Camera Component is null");
		return;
	}
	TObjectPtr<UC_StateComponent> StateComponent = Cast<UC_StateComponent>(MeshComp->GetOwner()->GetComponentByClass(UC_StateComponent::StaticClass()));
	if(StateComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, L"State Component is null");
		return;
	}
	StateComponent.Get()->SetIdleMode();
	Movement->EnableMove();
	Camera->DisableFixedCamera();
}

FString UAnimNotify_EndPlay::GetNotifyName_Implementation() const
{
	return "End Attack";
}
