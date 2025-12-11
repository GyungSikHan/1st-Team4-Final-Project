#pragma once

#include "CoreMinimal.h"
#include "AI/Base/BaseAI.h"
#include "Griffon.generated.h"

UCLASS()
class EMBER_API AGriffon : public ABaseAI
{
	GENERATED_BODY()

public:
	AGriffon();
	virtual void BeginPlay() override;
	// virtual void Tick(float DeltaTime) override;

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void HandleBeginOverlap(AActor* OtherActor) override;
	virtual void HandleEndOverlap(AActor* OtherActor) override;
};
