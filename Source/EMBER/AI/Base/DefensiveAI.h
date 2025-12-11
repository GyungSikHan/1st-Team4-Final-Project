#pragma once

#include "CoreMinimal.h"
#include "AI/Base/BaseAI.h"
#include "DefensiveAI.generated.h"


UCLASS()
class EMBER_API ADefensiveAI : public ABaseAI
{
	GENERATED_BODY()

public:
	ADefensiveAI();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//virtual void OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus) override;
};
