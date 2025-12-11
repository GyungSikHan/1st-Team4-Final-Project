#include "AI/CUserWidget_AIHP.h"
#include "AI/Base/BaseAI.h"
#include "GameFramework/PlayerController.h"

void UCUserWidget_AIHP::SetTargetAI(ABaseAI* InAI)
{
	TargetAI = InAI;

	if (TargetAI)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetAI Set: %s"), *TargetAI->GetName());
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}



