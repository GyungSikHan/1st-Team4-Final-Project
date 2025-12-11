#include "AI/Task/BTT_DragonMeteor.h"
#include "AI/Boss/Dragon.h"
#include "AI/CAIController.h"
#include "AI/AnimInstance/DragonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_DragonMeteor::UBTT_DragonMeteor()
{
	NodeName = TEXT("DragonMeteor");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_DragonMeteor::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	BTComp = &Comp;
	AIController = Cast<ACAIController>(Comp.GetAIOwner());
	Dragon = Cast<ADragon>(AIController ? AIController->GetPawn() : nullptr);
	
	if (!Dragon || !MeteorMontage)
		return EBTNodeResult::Failed;

	DragonAnim = Cast<UDragonAnimInstance>(Dragon->GetMesh()->GetAnimInstance());
	if (!DragonAnim)
		return EBTNodeResult::Failed;
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UBTT_DragonMeteor::OnMontageEnded);
	DragonAnim->Montage_SetEndDelegate(EndDelegate, MeteorMontage);

	const float PlayTime = DragonAnim->Montage_Play(MeteorMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);
	if (PlayTime == 0.f) return EBTNodeResult::Failed;

	DragonAnim->Montage_SetNextSection(FName("Fly"), FName("Glide"), MeteorMontage);
	DragonAnim->Montage_SetNextSection(FName("Glide"), FName("Land"), MeteorMontage);
	DragonAnim->Montage_JumpToSection(FName("Fly"), MeteorMontage);

	Dragon->SetLaunchZPower(LaunchZPower);
	Dragon->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Dragon->GetCharacterMovement()->GravityScale = 0.f;
	AIController->StopMovement();

	Dragon->SetActiveMeteorTask(this);
	
	return EBTNodeResult::InProgress;
}

void UBTT_DragonMeteor::FinishFromLanded()
{
	if (BTComp)
	{
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_DragonMeteor::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!BTComp || bInterrupted)
	{
		FinishLatentTask(*BTComp, EBTNodeResult::Failed);
		return;
	}

	FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}
