// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "EmberPlayerState.generated.h"

class UItemTemplate;
class UEmberAbilitySystemComponent;

UCLASS()
class EMBER_API AEmberPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEmberPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	//~APlayerState Overrides
	virtual void PostInitializeComponents() override;
	//~End of APlayerState Overrides

public:
	//~IAbilitySystemInterface Overrides
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface Overrides
	UEmberAbilitySystemComponent* GetEmberAbilitySystemComponent() const { return AbilitySystemComponent; }

public:
	UFUNCTION(Server, Reliable)
	void Server_AddInventoryItem(TSubclassOf<UItemTemplate> InItemTemplateClass, EItemRarity InItemRarity, int32 InItemCount);

public:
	void InitAbilitySystemComponent();
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Ember|PlayerState")
	TObjectPtr<UEmberAbilitySystemComponent> AbilitySystemComponent;
};
