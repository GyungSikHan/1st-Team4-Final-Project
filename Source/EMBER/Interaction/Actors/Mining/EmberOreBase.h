// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameData.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Interaction/Actors/EmberWorldInteractable.h"
#include "EmberOreBase.generated.h"

struct FChaosBreakEvent;
class AFieldSystemActor;
class UItemTemplate;

UCLASS()
class EMBER_API AEmberOreBase : public AEmberWorldInteractable
{
	GENERATED_BODY()

public:
	AEmberOreBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	void HandlePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* InDamageCauser);

	UFUNCTION()
	void HandleBreakEvent(const FChaosBreakEvent& BreakEvent);

private:
	void InitResource();

	void HandleRewardItem(int32 RewardItemCount);
	
	UFUNCTION(NetMulticast, Reliable)
	void SetDynamic(FVector Position);

protected:
	/* ChaosPhsics */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore")
	TObjectPtr<UChildActorComponent> AnchorField;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|ChaosPhsics")
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|ChaosPhsics")
	TSubclassOf<AFieldSystemActor> ChaosImpactClass;

	/* Resource */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|Resource")
	int MaxResourceCount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|Resource")
	float ConsumedResourceCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|Resource")
	float UnitElementPerResource = 0;

	float CurrentBrokenElements = 0;

	/* Item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|RewardItem")
	TSubclassOf<UItemTemplate> RewardItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|RewardItem")
	EItemRarity RewardItemRarity;

	int32 ItemID;
	
	/* Reward Event */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|Event")
	FGameplayTag EventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ore|Event")
	FGameplayEventData EventData;
	
private:
	UPROPERTY()
	AActor* DamageCauser;
};
