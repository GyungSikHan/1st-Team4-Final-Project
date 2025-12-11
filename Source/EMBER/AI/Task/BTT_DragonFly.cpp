#include "AI/Task/BTT_DragonFly.h"
#include "AI/Boss/Dragon.h"
#include "AI/AnimInstance/DragonAnimInstance.h"
#include "CAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/BehaviorTree/CBehaviorTreeComponent.h"

UBTT_DragonFly::UBTT_DragonFly()
{
	NodeName = TEXT("DragonFly");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTT_DragonFly::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("UBTT_DragonFly::ExecuteTask START"));
	BTComp = &Comp;
	
	AIController = Cast<ACAIController>(Comp.GetAIOwner());
	Dragon = Cast<ADragon>(AIController ? AIController->GetPawn() : nullptr);
	if (!Dragon)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Dragon is null"));
		return EBTNodeResult::Failed;
	}
	if (!FlyMontage)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FlyMontage is null"));
		return EBTNodeResult::Failed;
	}

	UDragonAnimInstance* DragonAnim = Cast<UDragonAnimInstance>(Dragon->GetMesh()->GetAnimInstance());
	if (!DragonAnim)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DragonAnim is null"));

		return EBTNodeResult::Failed;
	}
	
	Dragon->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Dragon->GetCharacterMovement()->GravityScale = 0.f;
	AIController->StopMovement();
	Dragon->SetLaunchZPower(LaunchZPower);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Before Montage_Play"));
	float Duration = DragonAnim->Montage_Play(FlyMontage);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Montage_Play Result: %.2f"), Duration));

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UBTT_DragonFly::OnMontageEnded);
	DragonAnim->Montage_SetEndDelegate(EndDelegate, FlyMontage);

	

	bMontagePlaying = true;

	return EBTNodeResult::InProgress;
}

void UBTT_DragonFly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTT_DragonFly::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (BTComp)
	{
		FinishLatentTask(*BTComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
	}

	bMontagePlaying = false;
}
