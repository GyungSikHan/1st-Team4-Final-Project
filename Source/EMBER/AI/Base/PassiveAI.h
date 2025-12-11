#pragma once

#include "CoreMinimal.h"
#include "AI/Base/BaseAI.h"
#include "PassiveAI.generated.h"

UCLASS()
class EMBER_API APassiveAI : public ABaseAI
{
	GENERATED_BODY()

public:
	APassiveAI();
	virtual void BeginPlay() override;

	// void CheckDetection(bool IsDetect);
	// void UpdateClosestActorTimer();
	//	virtual void OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus) override;


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDeath() override;
	TArray<AActor*> EnemyActors;
	AActor* ClosestActor;

	FTimerHandle UpdateDistanceTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClosestDistanceBoundary;

	float ClosestDistance;
};
