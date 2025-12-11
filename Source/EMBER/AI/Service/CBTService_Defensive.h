#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Defensive.generated.h"

UCLASS()
class EMBER_API UCBTService_Defensive : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Defensive();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="Action")
	float ActionRange = 150.f;

	UPROPERTY(EditAnywhere, Category="Debug")
	bool bDrawDebug;

private:
	bool bisHit;
	ACharacter* Target;
};
