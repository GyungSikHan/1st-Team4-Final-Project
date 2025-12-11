#include "AI/AnimInstance/Mammoth_AnimInstance.h"

void UMammoth_AnimInstance::AnimNotify_Idle()
{
	DesiredActionType = EAnimActionType::Idle;
}

void UMammoth_AnimInstance::AnimNotify_Run()
{
	DesiredActionType = EAnimActionType::Run;
}

void UMammoth_AnimInstance::AnimNotify_Eat()
{
	DesiredActionType = EAnimActionType::Eat;
}
