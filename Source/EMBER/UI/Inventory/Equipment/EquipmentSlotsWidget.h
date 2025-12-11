// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "EquipmentSlotsWidget.generated.h"

enum class EEquipmentSlotType : uint8;
class UCommonVisibilitySwitcher;
class UItemInstance;
class UItemSlotWidget;
class UEquipmentSlotHandWidget;
class UEquipmentSlotSingleWidget;
class UInventoryEquipmentManagerComponent;

USTRUCT(BlueprintType)
struct FEquipmentInitializeMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventoryEquipmentManagerComponent> InventoryEquipmentManager;
};

UCLASS()
class EMBER_API UEquipmentSlotsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEquipmentSlotsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UUserWidget Overrides
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget Overrides

private:
	void ConstructUI(FGameplayTag Channel, const FEquipmentInitializeMessage& Message);
	void DestructUI();

	void OnEquipmentEntryChanged(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance, int32 ItemCount);

public:
	UPROPERTY(EditAnywhere, meta=(Categories="Message"))
	FGameplayTag MessageChannelTag;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotHandWidget> Widget_Hand_Left;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotHandWidget> Widget_Hand_Right;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Armor_Helmet;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Armor_Chest;

	UPROPERTY()
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Armor_Shoulder;

	UPROPERTY()
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Armor_Cloak;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Armor_Gloves;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Armor_Pants;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Armor_Foot;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Utility_Primary;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Utility_Secondary;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Utility_Tertiary;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentSlotSingleWidget> Widget_Utility_Quaternary;

private:
	TMap<EEquipmentSlotType, TObjectPtr<UEquipmentSlotSingleWidget>> EquipmentSlotWidgets;
	
	UPROPERTY()
	TObjectPtr<UInventoryEquipmentManagerComponent> InventoryEquipmentManager;
	
private:
	FDelegateHandle EquipmentChangedDelegateHandle;
	FGameplayMessageListenerHandle MessageListenerHandle;
};
