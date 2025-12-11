#include "AI/Griffon_Controller.h"
#include "Component/C_StateComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/CBehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AGriffon_Controller::AGriffon_Controller()
{
	Sight->SightRadius = Sight_Range;
	Sight->LoseSightRadius = Sight_Range+300.0f;
	Sight->PeripheralVisionAngleDegrees = 90;
}

void AGriffon_Controller::BeginPlay()
{
	Super::BeginPlay();
}

void AGriffon_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AGriffon_Controller::OnUnPossess()
{
	Super::OnUnPossess();
}

void AGriffon_Controller::OnTargetPerceptionUpdated(AActor* Target, FAIStimulus Stimulus)
{
	Perception->GetCurrentlyPerceivedActors(nullptr, Actors);
	if (Stimulus.WasSuccessfullySensed() && Target->Tags.Contains(FName("Player")) && !Behavior->IsDetectMode() && !Behavior->IsCombatMode())
	{
		if (Behavior)
		{
			Blackboard->SetValueAsObject("TargetActor", Target);
			AIState->SetDetectMode();
			Blackboard->SetValueAsBool("IsDetect", true);

			UE_LOG(LogTemp, Warning, TEXT("Detected target"));
		}
	}
	else
	{
		if (Actors.Num() == 0 && !Behavior->IsDetectMode() && !Behavior->IsCombatMode())
		{
			Actors.RemoveSingle(Target);
			UE_LOG(LogTemp, Warning, TEXT("Perception: Target Lost"), Actors.Num());
			AIState->SetIdleMode();
			Blackboard->SetValueAsObject("TargetActor", nullptr);
			return;
		}
	}
}
