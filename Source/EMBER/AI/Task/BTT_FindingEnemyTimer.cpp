// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTT_FindingEnemyTimer.h"
#include "AI/CAIController.h"
#include "AI/Base/BaseAI.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_FindingEnemyTimer::UBTT_FindingEnemyTimer()
{
	NodeName = TEXT("Finder");
}

EBTNodeResult::Type UBTT_FindingEnemyTimer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//IsDetect 키 확인후 타이머 시작 or 정지
	Owner = &OwnerComp;
	UBlackboardComponent* Comp = OwnerComp.GetBlackboardComponent();
	bool bIsDetect = (Comp->GetValueAsBool("IsDetect"));
	CheckDetection(bIsDetect);
	return EBTNodeResult::InProgress;
}

void UBTT_FindingEnemyTimer::CheckDetection(bool IsDetect)
{
	if (IsDetect)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(UpdateDistanceTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(UpdateDistanceTimer, this,
			                                       &UBTT_FindingEnemyTimer::UpdateClosestActorTimer,
			                                       1.0f, true);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateDistanceTimer);
	}
}

void UBTT_FindingEnemyTimer::UpdateClosestActorTimer()
{
	ACAIController* Controller = Cast<ACAIController>(Owner->GetAIOwner());
	ABaseAI* AI = Controller ? Cast<ABaseAI>(Controller->GetPawn()) : nullptr;
	if (AI)
	{
		UBlackboardComponent* BlackboardComponent = Controller->GetBlackboardComponent();
		TArray<AActor*> EnemyActors = Controller->Actors;
		float ClosestDistance = FLT_MAX;

		if (EnemyActors.Num() == 0)
		{
			ClosestActor = nullptr;
			ClosestDistance = FLT_MAX;
			return;
		}

		FVector MyLocation = AI->GetActorLocation();
		for (AActor* Enemy : EnemyActors)
		{
			if (!Enemy->Tags.Contains(FName("Player"))) continue;
			FVector EnemyLocation = Enemy->GetActorLocation();

			//2d 평면에서만 계산
			MyLocation.Z = 0;
			EnemyLocation.Z = 0;

			float Distance = FVector::Dist(MyLocation, EnemyLocation);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Enemy;
				BlackboardComponent->SetValueAsObject("TargetActor", ClosestActor);
				// UE_LOG(LogTemp, Warning, TEXT("Closest Actor Updated: %s at distance %f"), *ClosestActor->GetName(),
				//        ClosestDistance);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer: AI is null"));
	}
}
