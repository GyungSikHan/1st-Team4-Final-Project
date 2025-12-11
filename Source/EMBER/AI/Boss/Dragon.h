#pragma once

#include "CoreMinimal.h"
#include "AI/Base/BaseAI.h"
#include "Dragon.generated.h"

class ACAIController;
class ADragonSpitProjectile;
class ADragonBreathActor;
class UBTT_DragonMeteor;
class ADragonMeteorSpawner;
class UNiagaraSystem;

UCLASS()
class EMBER_API ADragon : public ABaseAI
{
	GENERATED_BODY()

public:
	ADragon();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SpawnSpit();

	UFUNCTION()
	void SpawnBreath();

	UFUNCTION()
	void StopBreath();

	void Landed(const FHitResult& Hit) override;
	virtual void OnDeath() override;

	void SetLaunchZPower(float InZPower) { LaunchZPower = InZPower; }
	float GetLaunchZPower() const { return LaunchZPower; }

	void SetActiveMeteorTask(UBTT_DragonMeteor* Task) { CurrentMeteorTask = Task; }
	void StartMeteorPhase();
	void UpdateOrbit(float DeltaTime);
	bool HasStartedMeteorPhase() const { return bMeteorStarted; }

	AActor* GetTargetActor() { return TargetActor; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathSignature OnDeathDispatcher;
	
protected:
	virtual void HandleBeginOverlap(AActor* OtherActor) override;
	virtual void HandleEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditdefaultsOnly)
	TSubclassOf<ADragonSpitProjectile> SpitClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADragonBreathActor> BreathClass;

	UPROPERTY()
	ADragonBreathActor* CurrentBreath;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADragonMeteorSpawner> MeteorSpawnerClass; 

	UPROPERTY()
	ADragonMeteorSpawner* CurrentMeteorSpawner;

	UPROPERTY()
	UBTT_DragonMeteor* CurrentMeteorTask = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ACAIController> AIController;

	UPROPERTY(EditDefaultsOnly)
	UCBehaviorTreeComponent* BTComp;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* LandingEffect;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	float OrbitAngle = 0.f;
	
	UPROPERTY()
	FVector OrbitCenter;

	float LaunchZPower = 300.f;
	bool bMeteorStarted = false;
	bool bOrbiting = false;
};
