// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CheatEntryWidget.generated.h"

class UAnimMontage;
class USizeBox;
class UImage;
class UButton;
class UTextBlock;
class UItemTemplate;

UENUM(BlueprintType)
enum class ECheatEntryType : uint8
{
	None,
	PrimaryWeapon,
	SecondaryWeapon,
	Tool,
	Armor,
	Utility,
	Crafting,
	Animation
};

/**
 * 
 */
UCLASS()
class EMBER_API UCheatEntryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCheatEntryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	//~UUserWidget Overrides
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget Overrides

public:
	void InitializeUI(ECheatEntryType InCheatEntryType, TSubclassOf<UItemTemplate> InItemTemplateClass, TSoftObjectPtr<UAnimMontage> InAnimMontage);

private:
	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnButtonRightClicked();
	
public:
	UPROPERTY()
	ECheatEntryType CheatEntryType = ECheatEntryType::None;
	
	UPROPERTY()
	TSubclassOf<UItemTemplate> ItemTemplateClass;

	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> AnimMontage;

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Entry;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Entry;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Entry;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Entry;
};
