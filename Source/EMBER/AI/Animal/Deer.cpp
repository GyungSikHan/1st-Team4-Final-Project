#include "AI/Animal/Deer.h"

#include "AI/AnimInstance/BaseAIAnimInstance.h"
#include "AI/CAIController.h"
#include "AnimInstance/Deer_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

ADeer::ADeer()
{
	// MaxHP = 100;
	// CurrentHP = MaxHP;
	// bIsDie = false;
}

// float ADeer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
//                         AActor* DamageCauser)
// {
// 	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
// 	UDeer_AnimInstance* AnimInstance = Cast<UDeer_AnimInstance>(GetMesh()->GetAnimInstance());
// 	ACAIController* AIController = Cast<ACAIController>(GetController());
//
// 	GetController()->StopMovement();
// 	if (AnimInstance)
// 	{
// 		AnimInstance->StopAllMontages(0.5f);
// 		AnimInstance->PlayStateMontage(EAnimActionType::HitFront);
// 	}
//
//
// 	return ActualDamage;
// }
