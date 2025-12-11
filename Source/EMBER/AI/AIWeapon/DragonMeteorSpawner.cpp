#include "AI/AIWeapon/DragonMeteorSpawner.h"
#include "AI/Boss/Dragon.h"
#include "AIWeapon/DragonMeteorProjectile.h"

ADragonMeteorSpawner::ADragonMeteorSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADragonMeteorSpawner::Initialize(ADragon* InDragon)
{
	Dragon = InDragon;
	bInitialized = true;
}

void ADragonMeteorSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ADragonMeteorSpawner::SpawnMeteor,
		SpawnInterval,
		true);
}

void ADragonMeteorSpawner::SpawnMeteor()
{
	if (!bInitialized || !Dragon || Dragon->bIsDie)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		Destroy();
		return;
	}

	if (MeteorDataArray.Num() == 0) return;

	const int32 Index = FMath::RandRange(0, MeteorDataArray.Num() - 1);
	const FMeteorData& Data = MeteorDataArray[Index];

	FVector2D RandOffset = FMath::RandPointInCircle(SpawnRadius);
	FVector SpawnLocation = GetActorLocation() + FVector(RandOffset.X, RandOffset.Y, SpawnHeight);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ADragonMeteorProjectile* Meteor = GetWorld()->SpawnActor<ADragonMeteorProjectile>(
		DragonMeteorClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams);

	if (Meteor)
	{
		Meteor->InitializeMeteor(Data.Effect, Data.Damage, Data.CollisionRadius);
	}
}


