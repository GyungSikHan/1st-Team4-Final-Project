#include "BTT_Attack.h"
#include "AI/Base/BaseAI.h"
#include "AI/Boss/Griffon.h"
#include "CAIController.h"
#include "C_StateComponent.h"
#include "AnimInstance/BaseAIAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Player/EmberPlayerCharacter.h"
#include "AIComponent/CAIWeaponComponent.h"
#include "AIWeapon/CAI_Weapon.h"
#include "Kismet/GameplayStatics.h"


UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("AttackState");
	bNotifyTick = true;
	//bIsMontagePlaying = false;
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task Controller is null");
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, L"BlackboardComponent is null");
		return EBTNodeResult::Failed;
	}
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task ai is null");
		return EBTNodeResult::Failed;
	}
	AGriffon* Griffon = Cast<AGriffon>(ai);
	if (Griffon)
	{
		// UE_LOG(LogTemp, Error, L"Griffon Casting In Attack Task");
		AttackIndex = BlackboardComponent->GetValueAsInt("RandomPattern");
	}

	UCAIWeaponComponent* weapon = Cast<
		UCAIWeaponComponent>(ai->GetComponentByClass(UCAIWeaponComponent::StaticClass()));
	if (weapon == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task weapon is null");
		return EBTNodeResult::Failed;
	}

	controller->StopMovement();

	weapon->DoAction(AttackIndex);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task Controller is null");
		return EBTNodeResult::Failed;
	}
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task ai is null");
		return EBTNodeResult::Failed;
	}
	UCAIWeaponComponent* weapon = Cast<
		UCAIWeaponComponent>(ai->GetComponentByClass(UCAIWeaponComponent::StaticClass()));
	if (weapon == nullptr && weapon->GetDoAction() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task weapon is null");
		return EBTNodeResult::Failed;
	}

	bool bBeginAction = weapon->GetDoAction()->GetBeginAction();
	if (bBeginAction == false)
		weapon->GetDoAction()->Begin_DoAction();
	weapon->GetDoAction()->End_DoAction();

	return EBTNodeResult::Succeeded;
}

void UBTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task Controller is null");
		return;
	}
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task ai is null");
		return;
	}
	UCAIWeaponComponent* weapon = Cast<
		UCAIWeaponComponent>(ai->GetComponentByClass(UCAIWeaponComponent::StaticClass()));
	if (weapon == nullptr && weapon->GetDoAction() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task weapon is null");
		return;
	}
	UC_StateComponent* state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task state is null");
		return;
	}
	if (state->IsHittdMode() == true)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}
	bool bCheck = true;
	// UE_LOG(LogTemp, Warning, L"state %d", state->IsIdleMode());
	bCheck &= state->IsIdleMode();
	// UE_LOG(LogTemp, Warning, L"inaction %d", weapon->GetDoAction()->GetInAction());
	bCheck &= weapon->GetDoAction()->GetInAction() == false;

	if (bCheck == true)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}


//EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
//{
//	ACAIController* AIController = Cast<ACAIController>(Comp.GetAIOwner());
//	if (!AIController) return EBTNodeResult::Failed;
//	
//	ABaseAI* AICharacter = Cast<ABaseAI>(AIController->GetCharacter());
//	if (!AICharacter) return EBTNodeResult::Failed;
//
//	AActor* TargetActor = Cast<AActor>(Comp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
//	if (!TargetActor) return EBTNodeResult::Failed;
//
//	UBaseAIAnimInstance* AIAnimInstance = Cast<UBaseAIAnimInstance>(AICharacter->GetMesh()->GetAnimInstance());
//	if (!AIAnimInstance) return EBTNodeResult::Failed;
//
//	CachedAnimInstance = AIAnimInstance;
//	bIsMontagePlaying = true;
//
//	AIAnimInstance->OnMontageEnded.AddDynamic(this, &UBTT_Attack::OnMontageEnded);
//	
//	AICharacter->OnAttack();
//	
//	UGameplayStatics::ApplyDamage(TargetActor, AICharacter->GetAttackPower(), AIController, AICharacter, nullptr);
//	UE_LOG(LogTemp, Warning, L"Attack");
//	//???? ????????? ???? ??÷? Succes
//	return EBTNodeResult::Succeeded;
//	//return EBTNodeResult::InProgress;
//}
//
//void UBTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	if (!bIsMontagePlaying)
//	{
//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//	}
//}
//
//void UBTT_Attack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//	if (CachedAnimInstance)
//	{
//		CachedAnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTT_Attack::OnMontageEnded);
//	}
//
//	bIsMontagePlaying = false;
//}
