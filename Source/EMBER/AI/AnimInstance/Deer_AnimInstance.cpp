#include "AI/AnimInstance/Deer_AnimInstance.h"
#include "Animation/AnimMontage.h"

UDeer_AnimInstance::UDeer_AnimInstance()
{
}

void UDeer_AnimInstance::PlayTurnMontage(bool direction)
{
	UAnimMontage* TurnMontage;
	if (direction)
	{
		TurnMontage = LeftTurnMontage;
		Montage_Play(LeftTurnMontage);
	}
	else
	{
		TurnMontage = RightTurnMontage;
		Montage_Play(RightTurnMontage);
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUFunction(this, FName("OnTurnMontageEnded"));
	Montage_SetEndDelegate(EndDelegate, TurnMontage);
}

void UDeer_AnimInstance::OnTurnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	StopMontage();
}

void UDeer_AnimInstance::StopMontage()
{
	StopAllMontages(0.1f); // 블렌드 아웃(0.1초) 후 애니메이션 중지
}