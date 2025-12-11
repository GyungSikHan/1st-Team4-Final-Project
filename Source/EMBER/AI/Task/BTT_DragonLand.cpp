#include "AI/Task/BTT_DragonLand.h"
#include "AI/Boss/Dragon.h"
#include "AI/AnimInstance/DragonAnimInstance.h"
#include "CAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTT_DragonLand::UBTT_DragonLand()
{
	NodeName = TEXT("DragonLand");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_DragonLand::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("UBTT_DragonLand::ExecuteTask"));
	BTComp = &Comp;

	AIController = Cast<ACAIController>(Comp.GetAIOwner());
	Dragon = Cast<ADragon>(AIController ? AIController->GetPawn() : nullptr);
	if (!Dragon || !LandMontage) return EBTNodeResult::Failed;

	UDragonAnimInstance* DragonAnim = Cast<UDragonAnimInstance>(Dragon->GetMesh()->GetAnimInstance());
	if (!DragonAnim) return EBTNodeResult::Failed;

	Dragon->GetCharacterMovement()->GravityScale = 1.f;
	DragonAnim->Montage_Play(LandMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UBTT_DragonLand::OnMontageEnded);
	DragonAnim->Montage_SetEndDelegate(EndDelegate, LandMontage);

	bMontagePlaying = true;

	return EBTNodeResult::InProgress;
}

void UBTT_DragonLand::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTT_DragonLand::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("UBTT_DragonLand::OnMontageEnded"));

	if (Dragon)
	{
		if (UCBehaviorTreeComponent* AIState = Cast<UCBehaviorTreeComponent>(Dragon->GetComponentByClass(UCBehaviorTreeComponent::StaticClass())))
		{
			AIState->SetNormalAttackMode();
		}
	}
	if (BTComp)
	{
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("UBTT_DragonLand::FinishLatentTask"));

	}
	
	bMontagePlaying = false;
}
