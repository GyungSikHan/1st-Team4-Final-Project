// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameData.h"
#include "Interaction/Actors/EmberWorldInteractable.h"
#include "Notify/AnimNotifyState_SendGameplayEvent.h"
#include "Physics/Experimental/ChaosEventType.h"
#include "EmberTreeBase.generated.h"

class AFieldSystemActor;
class UItemTemplate;
struct FEmberInteractionQuery;

UCLASS()
class EMBER_API AEmberTreeBase : public AEmberWorldInteractable
{
	GENERATED_BODY()

public:
	AEmberTreeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree")
	TObjectPtr<UChildActorComponent> AnchorField;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|ChaosPhsics")
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|ChaosPhsics")
	TSubclassOf<AFieldSystemActor> ChaosImpactClass;

	/* Resource */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|Resource")
	int MaxResourceCount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|Resource")
	float MassDestructionThresholdRatio = 0.9f;
	
	UPROPERTY(EditDefaultsOnly)
	float UnitMassPerResource = 0.f;

	float ConsumedResourceCount = 0.f;
	float TotalGeometryMass = 0.f;
	float CurrentBrokenMass = 0.f;
	
	/* Item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|RewardItem")
	TSubclassOf<UItemTemplate> RewardItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|RewardItem")
	EItemRarity RewardItemRarity;

	int32 CachedItemID;
	
	/* Reward Event */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|Event")
	FGameplayTag EventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree|Event")
	FGameplayEventData EventData;
	
private:
	UPROPERTY()
	AActor* DamageCauser;
};
