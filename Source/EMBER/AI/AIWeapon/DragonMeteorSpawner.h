#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonMeteorSpawner.generated.h"

class ADragon;
class ADragonMeteorProjectile;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FMeteorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* Effect = nullptr;

	UPROPERTY(EditAnywhere)
	float CollisionRadius = 50.f;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
};

UCLASS()
class EMBER_API ADragonMeteorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ADragonMeteorSpawner();
	void Initialize(ADragon* InDragon);

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Category = "Meteor")
	TArray<FMeteorData> MeteorDataArray;

	UPROPERTY(EditAnywhere, Category = "Meteor")
	TSubclassOf<ADragonMeteorProjectile> DragonMeteorClass;

	UPROPERTY(EditAnywhere, Category = "Meteor")
	float SpawnInterval = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Meteor")
	float SpawnRadius = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Meteor")
	float SpawnHeight = 1500.f;

	UPROPERTY()
	ADragon* Dragon;

	FTimerHandle SpawnTimerHandle;
	bool bInitialized = false;

	void SpawnMeteor();
};
