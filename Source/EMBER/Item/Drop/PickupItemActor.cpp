#include "Item/Drop/PickupItemActor.h"
#include "Net/UnrealNetwork.h"
#include "Item/ItemTemplate.h"
#include "Item/ItemInstance.h"
#include "UI/Data/EmberItemData.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"

APickupItemActor::APickupItemActor()
{
	RarityEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RarityEffectComponent"));
	RarityEffectComponent->SetupAttachment(GetRootComponent());
}

void APickupItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APickupItemActor, LootContent, COND_None);
}

void APickupItemActor::InitializeLootDrop(const FLootResultData& InLootData)
{
	if (HasAuthority())
	{
		LootContent = InLootData;

		if (IsNetMode(NM_ListenServer) || GetNetMode() == NM_Standalone)
		{
			OnRep_LootContent();
		}
	}
}

void APickupItemActor::OnRep_LootContent()
{
	if (LootContent.ItemTemplateClass == nullptr)
	{
		Destroy();
		return;
	}

	UItemInstance* ItemInstance = NewObject<UItemInstance>(this);
	if (ItemInstance)
	{
		int32 ItemID = UEmberItemData::Get().FindItemTemplateIDByClass(LootContent.ItemTemplateClass);
		const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemID);
		ItemInstance->Init(ItemID, LootContent.Rarity, LootContent.Quantity);
		PickupInfo.ItemInstance = ItemInstance;
		InteractionInfo.Title = ItemTemplate.DisplayName;
		
		OnRep_PickupInfo();
	}

	if (RarityEffectComponent)
	{
		if (const FLinearColor* Color = RarityColorMap.Find(LootContent.Rarity))
		{
			RarityEffectComponent->SetColorParameter(FName("RarityColor"), *Color);
			UE_LOG(LogTemp, Log, TEXT("APickupItemActor: Set Niagara color for rarity %s"), *UEnum::GetValueAsString(LootContent.Rarity));
		}
		else
		{
			RarityEffectComponent->SetColorParameter(FName("RarityColor"), FLinearColor::White);
		}
	}
}