// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTT_FlyMoveTo.h"
#include "AI/Base/BaseAI.h"
#include "AI/CAIController.h"
#include "AnimInstance/Griffon_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_FlyMoveTo::UBTT_FlyMoveTo()
{
	NodeName = "FlyMoveTo";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_FlyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BaseAI = Cast<ABaseAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (!BaseAI) return EBTNodeResult::Failed;
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	FBlackboard::FKey KeyID = BlackboardKey.GetSelectedKeyID();
	if (KeyID == FBlackboard::InvalidKey)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Blackboard Key"));
		return EBTNodeResult::Failed;
	}

	TSubclassOf<UBlackboardKeyType> KeyTypeClass = BlackboardComp->GetKeyType(KeyID);
	if (!KeyTypeClass)
	{
		UE_LOG(LogTemp, Error, TEXT("KeyType is null"));
		return EBTNodeResult::Failed;
	}

	// UE_LOG(LogTemp, Warning, TEXT("KeyType: %s"), *KeyTypeClass->GetName());

	if (KeyTypeClass == UBlackboardKeyType_Vector::StaticClass())
	{
		TargetLocation = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);
	}
	else if (KeyTypeClass == UBlackboardKeyType_Object::StaticClass())
	{
		TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));
		TargetLocation = TargetActor->GetActorLocation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, L"Wrong Type in FlyMoveTO")
	}

	CurrentLocation = BaseAI->GetActorLocation();
	Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	BaseAI->GetCharacterMovement()->MaxAcceleration = 700.0f;
	
	FRotator NewRotation = Direction.Rotation();
	BaseAI->SetActorRotation(NewRotation);

	return EBTNodeResult::InProgress;
}

bool UBTT_FlyMoveTo::IsNearGround()
{
	if (!BaseAI) return false;

	if (BlackboardComp->GetValueAsBool("IsHalfHP"))
	{
		return false;
	}

	FVector Start = BaseAI->GetActorLocation();
	FVector End = Start - FVector(0, 0, AcceptableRadius + 150.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(BaseAI);

	FHitResult Hit;
	bool bHit = BaseAI->GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	DrawDebugLine(
		BaseAI->GetWorld(),
		Start,
		End,
		bHit ? FColor::Green : FColor::Red, // 맞으면 초록, 아니면 빨강
		false,
		0.1f,
		0,
		2.0f
	);


	return bHit;
}
bool UBTT_FlyMoveTo::IsNearTargetLocation()
{
	CurrentLocation = BaseAI->GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation, TargetLocation);
	return Distance <= AcceptableRadius;
}

void UBTT_FlyMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	Direction = (TargetLocation - CurrentLocation).GetSafeNormal(); 
	BaseAI->AddMovementInput(Direction, 1.f);
	
	if (BaseAI->GetCharacterMovement()->Velocity.Size() > BlackboardComp->GetValueAsFloat("FlySpeed"))
	{
		BaseAI->GetCharacterMovement()->MaxAcceleration = 100.0f;
	}

	if (IsNearGround())
	{
		UE_LOG(LogTemp, Warning, TEXT("On Ground"));
		BaseAI->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		BaseAI->GetCharacterMovement()->GravityScale = 1.0f;

		UGriffon_AnimInstance* AnimInstance = Cast<UGriffon_AnimInstance>(BaseAI->GetMesh()->GetAnimInstance());
		AnimInstance->PlayLandingAnimation();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	if (IsNearTargetLocation())
	{
		UE_LOG(LogTemp, Warning, TEXT("FlyMove Success"));
		BaseAI->GetCharacterMovement()->MaxAcceleration = 0.0f;
		BaseAI->GetCharacterMovement()->MaxFlySpeed = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
