#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonMeteorProjectile.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UDecalComponent;

UCLASS()
class EMBER_API ADragonMeteorProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ADragonMeteorProjectile();
	void InitializeMeteor(UNiagaraSystem* InEffect, float InDamage, float InCollisionRadius);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* MeteorCollision;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* MeteorEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonSpit|VFX")
	UNiagaraSystem* ImpactNiagara;

	UPROPERTY(VisibleAnywhere)
	UDecalComponent* MeteorDecal;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere)
	float MaxDecalSize = 300.f;

	UPROPERTY(EditAnywhere)
	float DecalGrowthSpeed = 200.f;

	float Damage = 0.f;
	float TargetDecalSize = 0.f;
	float CurrentDecalSize = 0.f;
	float GroundZ = 0.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
