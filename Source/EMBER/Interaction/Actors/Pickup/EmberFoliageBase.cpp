// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberFoliageBase.h"

#include "EmberInteractionActorBase.h"
#include "EmberPickupable.h"
#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "UI/Data/EmberItemData.h"

AEmberFoliageBase::AEmberFoliageBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	
	InteractionInfo.bVisible = true;
	PrimaryActorTick.bCanEverTick = false;
}

void AEmberFoliageBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == false)
		return;

	if (IsValid(ItemTemplateClass) == false)
		Destroy();

	int32 ItemID = UEmberItemData::Get().FindItemTemplateIDByClass(ItemTemplateClass);
	if (ItemID == INDEX_NONE)
		Destroy();

	if (UItemInstance* ItemInstance = NewObject<UItemInstance>())
	{
		ItemInstance->Init(ItemID, EItemRarity::Common);
			
		FPickupInfo NewPickupInfo;
		NewPickupInfo.ItemInstance = ItemInstance;
		SetPickupInfo(NewPickupInfo);
	}
}
