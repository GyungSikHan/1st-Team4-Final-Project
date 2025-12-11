#pragma once

#include "CoreMinimal.h"
#include "Interaction/Actors/Pickup/EmberInteractionActorBase.h"
#include "Item/Drop/LootTable.h"
#include "PickupItemActor.generated.h"

class UItemTemplate;
class UItemInstance;
class UUserWidget;
class UNiagaraComponent;

UCLASS()
class EMBER_API APickupItemActor : public AEmberInteractionActorBase
{
	GENERATED_BODY()

public:
	APickupItemActor();

	void InitializeLootDrop(const FLootResultData& InLootData);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_LootContent();

	UPROPERTY(ReplicatedUsing = OnRep_LootContent)
	FLootResultData LootContent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> RarityEffectComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PickupItem|VFX")
	TMap<EItemRarity, FLinearColor> RarityColorMap;
};