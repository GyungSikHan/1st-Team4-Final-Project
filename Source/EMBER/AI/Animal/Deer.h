#pragma once

#include "CoreMinimal.h"
#include "AI/Base/PassiveAI.h"
#include "Deer.generated.h"


UCLASS()
class EMBER_API ADeer : public APassiveAI
{
	GENERATED_BODY()

public:
	ADeer();

	// virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
