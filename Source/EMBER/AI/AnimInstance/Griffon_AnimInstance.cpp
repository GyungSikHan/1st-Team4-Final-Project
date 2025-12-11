#include "AI/AnimInstance/Griffon_AnimInstance.h"

#include "C_CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "AI/Base/BaseAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


void UGriffon_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UGriffon_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UGriffon_AnimInstance::PlayLandingAnimation()
{
	Montage_Play(LandingMontage);
}

void UGriffon_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (ABaseAI* AI = Cast<ABaseAI>(TryGetPawnOwner()))
	{
		FVector Velocity = AI->GetVelocity();
		CurrentFlySpeed = FVector(Velocity.X, Velocity.Y, 0).Size();
		if (Controller = Cast<ACAIController>(AI->GetController()))
		{
			if (Blackboard = Cast<UBlackboardComponent>(Controller->GetBlackboardComponent()))
			{
				if (AI->GetCharacterMovement()->MovementMode == MOVE_Flying)
				{
					bIsAirborne = true;
				}
				else
				{
					bIsAirborne = false;
				}
			}
		}
	}
}
