// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "EmberPlayerController.generated.h"

enum class EWeaponSlotType : uint8;
class UEmberAbilitySystemComponent;
class UInputMappingContext; 
class UInputAction; 
class UItemTemplate;

UCLASS()
class EMBER_API AEmberPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	AEmberPlayerController();

protected:
	//~APlayerController Overrides
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController Overrides
	
private:
	UEmberAbilitySystemComponent* GetEmberAbilitySystemComponent() const;
	
public:
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(EWeaponSlotType WeaponSlotType, TSubclassOf<UItemTemplate> ItemTemplateClass);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;
	virtual void BeginPlay() override;
	
};

