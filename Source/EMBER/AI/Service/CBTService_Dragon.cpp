#include "AI/Service/CBTService_Dragon.h"
#include "AI/CAIController.h"
#include "AI/Boss/Dragon.h"
#include "AIComponent/CAIWeaponComponent.h"
#include "BehaviorTree/CBehaviorTreeComponent.h"
#include "AIWeapon/CAI_Weapon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StatusComponent.h"

UCBTService_Dragon::UCBTService_Dragon()
{
	Interval = 1.0f;
	RandomDeviation = 0.0f;
	TargetMemoryTime = 10.0f;
	LastSeenTime = 0.0f;
	CachedTarget = nullptr;
}

void UCBTService_Dragon::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<ACAIController> Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (Controller.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Controller is null");
		return;
	}
	TObjectPtr<ADragon> AI = Cast<ADragon>(Controller->GetPawn());
	if (AI.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI is null");
		return;
	}

	TObjectPtr<UCBehaviorTreeComponent> AIState = Cast<UCBehaviorTreeComponent>(AI->GetComponentByClass(UCBehaviorTreeComponent::StaticClass()));
	if (AIState.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AIstate is null");
		return;
	}

	TObjectPtr<UCAIWeaponComponent> WeaponComponent = Cast<UCAIWeaponComponent>(AI->GetComponentByClass(UCAIWeaponComponent::StaticClass()));
	if (WeaponComponent.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"WeaponComponent is null");
		return;
	}

	TObjectPtr<ACAI_Weapon> Weapon = WeaponComponent->GetDoAction();
	if (Weapon.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Weapon is null");
		return;
	}

	TObjectPtr<UBlackboardComponent> Blackboard = Controller->GetBlackboardComponent();

	if (Blackboard.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Blackboard is null");
		return;
	}

	if (bDrawDebug)
	{
		FVector start = AI->GetActorLocation();
		start.Z -= 25;

		FVector end = start;

		DrawDebugCylinder(AI->GetWorld(), start, end, ActionRange, 10, FColor::Red, false, Interval);
		DrawDebugCylinder(AI->GetWorld(), start, end, MeleeRange, 10, FColor::Blue, false, Interval);
	}
	UC_StateComponent* state = Cast<UC_StateComponent>(AI->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Dragon Service, state is null");
		return;
	}

	if (state->IsDeadMode() == true)
	{
		AIState->SetDeadMode();
		return;
	}
	if (state->IsHittdMode() == true)
	{
		AIState->SetHittedMode();
		return;
	}

	UStatusComponent* Status = AI->GetStatusComponent();
	if (Status && !Blackboard->GetValueAsBool("IsHalfHP"))
	{
		if (Status->GetHp() < Status->GetMaxHp() * 0.5f)
		{
			Blackboard->SetValueAsBool("IsHalfHP", true);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Half HP"));
		}
	}
	if (Status && !Blackboard->GetValueAsBool("IsMeteorPhase"))
	{
		if (Status->GetHp() <= Status->GetMaxHp() * 0.3f)
		{
			Blackboard->SetValueAsBool("IsMeteorPhase", true);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Meteor Phase triggered"));

		}
	}

	//Target이 없으면 Patrol
	TObjectPtr<ACharacter> Target = AIState->GetTarget();
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (Target)
	{
		CachedTarget = Target;
		LastSeenTime = CurrentTime;
	}
	else
	{
		if (CachedTarget && (CurrentTime - LastSeenTime <= TargetMemoryTime))
		{
			Target = CachedTarget;
			AIState->SetTarget(CachedTarget);
		}
		else
		{
			CachedTarget = nullptr;
			AIState->SetPatrolMode();
			return;
		}
	}

	//Target과 가까우면 AttackMode
	float distance = AI.Get()->GetDistanceTo(Target);
	int32 AttackCount = Weapon->GetAttackStack();
	int32 MissCount = Weapon->GetMissStack();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Distance: %.1f | AttackCount: %d | MissCount: %d"), distance, AttackCount, MissCount));

	if (distance < ActionRange || (AttackCount >= 3 && distance > MeleeRange))
	{
		AIState->SetActionMode();

		if (AttackCount >= 3)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Special Attack Block"));

			if (distance <= MeleeRange)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Combo Attack Triggered"));

				AIState->SetComboAttackMode();
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Spit Attack Triggered"));

				AIState->SetSpitAttackMode();
			}
		}
		else if (MissCount >= 10)
		{
			AIState->SetBreathAttackMode();
			Weapon->ResetMissStack();
		}
		else
		{
			AIState->SetNormalAttackMode();
			int32 RandomValue = FMath::RandRange(0, 2);
			Blackboard->SetValueAsInt("NormalAttackIdx", RandomValue);
		}

		if (!Weapon->GetDidAttack())
		{
			Weapon->IncreaseMissStack();
		}
	}
	else
	{
		AIState->SetChaseMode();
	}
	if (bDrawDebug == true)
	{

		DrawDebugSphere(
			GetWorld(),
			AI->GetActorLocation(),
			ActionRange,
			32,
			FColor::Blue,
			false,
			-1.f,
			0,
			2.f
		);

		DrawDebugSphere(
			GetWorld(),
			AI->GetActorLocation(),
			MeleeRange,
			32,
			FColor::Red,
			false,
			-1.f,
			0,
			2.f
		);
	}
}


