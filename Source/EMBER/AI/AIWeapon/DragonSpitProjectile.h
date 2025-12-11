#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonSpitProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class EMBER_API ADragonSpitProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ADragonSpitProjectile();
	void SetTargetActor(AActor* Target);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonSpit|Damage")
	float Damage = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonSpit|VFX")
	UNiagaraSystem* SpitNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonSpit|VFX")
	UNiagaraSystem* ImpactNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonSpit|VFX")
	UNiagaraComponent* SpitEffect;

	UPROPERTY()
	USphereComponent* SpitCollision;

	UPROPERTY()
	UProjectileMovementComponent* SpitMovement;

private:
	AActor* TargetActor = nullptr;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
