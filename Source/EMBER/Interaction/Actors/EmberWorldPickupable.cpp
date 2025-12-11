// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberWorldPickupable.h"

#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "Net/UnrealNetwork.h"
#include "System/EmberAssetManager.h"
#include "UI/Data/EmberItemData.h"

AEmberWorldPickupable::AEmberWorldPickupable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AEmberWorldPickupable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PickupInfo);
}

FEmberInteractionInfo AEmberWorldPickupable::GetPreInteractionInfo(const FEmberInteractionQuery& InteractionQuery) const
{
	return InteractionInfo;
}

UMeshComponent* AEmberWorldPickupable::GetMeshComponent() const
{
	return MeshComponent;
}

void AEmberWorldPickupable::SetPickupInfo(const FPickupInfo& InPickupInfo)
{
	if (HasAuthority() == false)
		return;

	if (InPickupInfo.ItemInstance)
	{
		PickupInfo = InPickupInfo;
		OnRep_PickupInfo();
	}
	else
	{
		Destroy();
	}
}

void AEmberWorldPickupable::OnRep_PickupInfo()
{
	TSoftObjectPtr<UStaticMesh> PickupableMeshPath = nullptr;
	
	if (const UItemInstance* ItemInstance = PickupInfo.ItemInstance)
	{
		const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
		PickupableMeshPath = ItemTemplate.PickupableMesh;
	}

	if (MeshComponent && MeshComponent->GetStaticMesh() == nullptr && PickupableMeshPath.IsNull() == false)
	{
		if (UStaticMesh* PickupableMesh = UEmberAssetManager::GetAssetByPath(PickupableMeshPath))
		{
			MeshComponent->SetStaticMesh(PickupableMesh);
		}
	}
}
