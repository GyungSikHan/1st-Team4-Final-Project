// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemInstance.h"

#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"
#include "UI/Data/EmberItemData.h"
#include "ItemTemplate.h"

UItemInstance::UItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemTemplateID);
	DOREPLIFETIME(ThisClass, ItemRarity);
	DOREPLIFETIME(ThisClass, ItemCount);
}

void UItemInstance::Init(int32 InItemTemplateID, EItemRarity InItemRarity)
{
	Init(InItemTemplateID, InItemRarity, 1);
}

void UItemInstance::Init(int32 InItemTemplateID, EItemRarity InItemRarity, int32 InItemCount)
{
	if (InItemTemplateID <= INDEX_NONE || InItemRarity == EItemRarity::Count || InItemCount <= INDEX_NONE)
		return;

	ItemTemplateID = InItemTemplateID;
	ItemRarity = InItemRarity;
	ItemCount = InItemCount;
	
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);
	for (const UItemFragment* Fragment : ItemTemplate.Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(this);
		}
	}
}

void UItemInstance::Init(TSubclassOf<UItemTemplate> InItemTemplate, EItemRarity InItemRarity, int32 InItemCount)
{
	int32 NewItemTempalteID = UEmberItemData::Get().FindItemTemplateIDByClass(InItemTemplate);
	Init(NewItemTempalteID, InItemRarity, InItemCount);
}

void UItemInstance::AddOrRemoveStatTagStack(FGameplayTag StatTag, int32 StackCount)
{
	AttributeContainer.AddOrRemoveStack(StatTag, StackCount);
}

int32 UItemInstance::GetStatCountByTag(FGameplayTag StatTag) const
{
	return AttributeContainer.GetStackCount(StatTag);
}

const UItemFragment* UItemInstance::FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const
{
	if (ItemTemplateID > INDEX_NONE && FragmentClass)
	{
		const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemTemplateID);
		return ItemTemplate.FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
