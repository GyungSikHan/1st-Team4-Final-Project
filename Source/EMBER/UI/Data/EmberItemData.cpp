// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberItemData.h"

#include "Item/Fragments/ItemFragment_Equipable_Armor.h"
#include "Item/Fragments/ItemFragment_Equipable_Weapon.h"
#include "System/EmberAssetManager.h"
#include "UObject/ObjectSaveContext.h"
#include "ItemTemplate.h"


const UEmberItemData& UEmberItemData::Get()
{
	return UEmberAssetManager::Get().GetItemData();
}

#if WITH_EDITORONLY_DATA
void UEmberItemData::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	ItemTemplateIDToClass.KeySort([](const int32 A, const int32 B)
	{
		return A < B;
	});

	ItemTemplateClassToID.Empty();

	for (const auto& Pair : ItemTemplateIDToClass)
	{
		ItemTemplateClassToID.Emplace(Pair.Value, Pair.Key);

		const UItemTemplate* ItemTemplate = Pair.Value.GetDefaultObject();
		if (const UItemFragment_Equipable_Weapon* WeaponFragment = ItemTemplate->FindFragmentByClass<UItemFragment_Equipable_Weapon>())
		{
			if (WeaponFragment->WeaponType != EWeaponType::Unarmed)
			{
				WeaponItemTemplateClasses.Add(Pair.Value);
			}
		}
		else if (ItemTemplate->FindFragmentByClass<UItemFragment_Equipable_Armor>())
		{
			ArmorItemTemplateClasses.Add(Pair.Value);
		}
	}
}
#endif // WITH_EDITORONLY_DATA

const UItemTemplate& UEmberItemData::FindItemTemplateByID(int32 ItemTemplateID) const
{
	const TSubclassOf<UItemTemplate>* ItemTemplateClass = ItemTemplateIDToClass.Find(ItemTemplateID);
	ensureAlwaysMsgf(ItemTemplateClass, TEXT("Can't find ItemTemplateClass from ID [%d]"), ItemTemplateID);
	return *(ItemTemplateClass->GetDefaultObject());
}

int32 UEmberItemData::FindItemTemplateIDByClass(TSubclassOf<UItemTemplate> ItemTemplateClass) const
{
	const int32* ItemTemplateID = ItemTemplateClassToID.Find(ItemTemplateClass);
	ensureAlwaysMsgf(ItemTemplateID, TEXT("Can't find ItemTemplateID from Class"));
	return *ItemTemplateID;
}

void UEmberItemData::GetAllItemTemplateClasses(TArray<TSubclassOf<UItemTemplate>>& OutItemTemplateClasses) const
{
	OutItemTemplateClasses.Reset();
	OutItemTemplateClasses.Reserve(ItemTemplateIDToClass.Num());
	
	for (auto& Pair : ItemTemplateIDToClass)
	{
		OutItemTemplateClasses.Add(Pair.Value);
	}
}
