#include "AI/Task/BTT_DragonAttack.h"
#include "AI/Boss/Dragon.h"
#include "CAIController.h"
#include "AI/AnimInstance/DragonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/BehaviorTree/CBehaviorTreeComponent.h"
#include "TimerManager.h"


UBTT_DragonAttack::UBTT_DragonAttack()
{
	NodeName = TEXT("DragonRangedAttack");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_DragonAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	BTComp = &Comp;
	AIController = Cast<ACAIController>(Comp.GetAIOwner());
	Dragon = Cast<ADragon>(AIController ? AIController->GetPawn() : nullptr);
	
	if (!Dragon || !RangedAttackMontage)
		return EBTNodeResult::Failed;

	DragonAnim = Cast<UDragonAnimInstance>(Dragon->GetMesh()->GetAnimInstance());
	if (!DragonAnim)
		return EBTNodeResult::Failed;
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UBTT_DragonAttack::OnMontageEnded);
	DragonAnim->Montage_SetEndDelegate(EndDelegate, RangedAttackMontage);

	const float PlayTime = DragonAnim->Montage_Play(RangedAttackMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
	if (PlayTime == 0.f) return EBTNodeResult::Failed;
	
	DragonAnim->Montage_SetNextSection(FName("Fly"), FName("Attack"), RangedAttackMontage);
	DragonAnim->Montage_SetNextSection(FName("Attack"), FName("Land"), RangedAttackMontage);
	DragonAnim->Montage_JumpToSection(FName("Fly"), RangedAttackMontage);

	Dragon->SetLaunchZPower(LaunchZPower);
	Dragon->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Dragon->GetCharacterMovement()->GravityScale = 0.f;
	AIController->StopMovement();

	float TotalLength = 0.f;
	const int32 SectionCount = RangedAttackMontage->CompositeSections.Num();
	for (int32 i = 0; i < SectionCount; ++i)
	{
		TotalLength += RangedAttackMontage->GetSectionLength(i);
	}

	Dragon->GetWorldTimerManager().SetTimer(
		MontageTimeoutHandle,
		this,
		&UBTT_DragonAttack::OnMontageTimeout,
		TotalLength,
		false
		);
	
	return EBTNodeResult::InProgress;
}

void UBTT_DragonAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{	
	if (!BTComp || bInterrupted)
	{
		FinishLatentTask(*BTComp, EBTNodeResult::Failed);
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("FinishDragonAttackMontage")));
	FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}

void UBTT_DragonAttack::OnMontageTimeout()
{
	if (BTComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("DragonAttackTaskTimeOut")));
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
}


