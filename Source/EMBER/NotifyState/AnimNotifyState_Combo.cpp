// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState/AnimNotifyState_Combo.h"

#include "Managers/EquipmentManagerComponent.h"

FString UAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
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

	UEquipmentManagerComponent* Equipment = Cast<UEquipmentManagerComponent>(MeshComp->GetOwner()->GetComponentByClass(UEquipmentManagerComponent::StaticClass()));
	if (Equipment == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Equipment Component is null");
		return;
	}
	//Equipment->GetAttackIndex();
	
}

void UAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

