#include "BTD_IsWithinOriginArea.h"
#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsWithinOriginArea::UBTD_IsWithinOriginArea()
{
}

bool UBTD_IsWithinOriginArea::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComponent) return false;

	FVector CurrentLocation = AIController->GetPawn()->GetActorLocation();
	FVector OriginLocation = BlackboardComponent->GetValueAsVector("OriginLocation");
	float Distance = FVector::Dist(CurrentLocation, OriginLocation);

#if WITH_EDITOR
	DrawDebugSphere(
		GetWorld(),
		OriginLocation,
		MaxDistance,
		12,
		FColor::Red,
		false,
		2.0f
	);
#endif
	
	return Distance <= MaxDistance;
}
