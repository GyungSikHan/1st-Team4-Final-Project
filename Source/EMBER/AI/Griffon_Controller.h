#pragma once

#include "CoreMinimal.h"
#include "AI/CAIController.h"
#include "Griffon_Controller.generated.h"


UCLASS()
class EMBER_API AGriffon_Controller : public ACAIController
{
	GENERATED_BODY()

public:

	AGriffon_Controller();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void OnTargetPerceptionUpdated(AActor* Target, FAIStimulus Stimulus) override;


	UPROPERTY(EditAnywhere)
	float Sight_Range = 4000.0f;
};
