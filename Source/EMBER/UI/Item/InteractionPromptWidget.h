#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Drop/LootTable.h"
#include "InteractionPromptWidget.generated.h"

class UVerticalBox;
class UTextBlock;

UCLASS()
class EMBER_API UInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction Prompt")
	void PopulateLootList(const TArray<FLootResultData>& LootData);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> LootListContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionText;
};