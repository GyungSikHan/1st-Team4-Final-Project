#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonBreathActor.generated.h"

struct FDamageData;
class ADragon;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class EMBER_API ADragonBreathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADragonBreathActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonBreath|Collision")
	float BoxLength = 1200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonBreath|Collision")
	float BoxWidth = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonBreath|Collision")
	float BoxHeight = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragonBreath|VFX")
	UNiagaraSystem* BreathEffect;

	UPROPERTY()
	UNiagaraComponent* BreathEffectComponent;

	UPROPERTY()
	ADragon* Dragon;

	UPROPERTY(EditAnywhere, Category = "DragonBreath|HitData")
	TArray<FDamageData> HitDatas;

private:
	TSet<AActor*> DamagedActors;
	FTimerHandle DestroyHandle;
	int32 CurrAttackIndex;

	void SprayDamage();
};
