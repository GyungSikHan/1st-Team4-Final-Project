#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_AIHP.generated.h"

class ABaseAI;

UCLASS()
class EMBER_API UCUserWidget_AIHP : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHP(float InMaxHP,float InHP);

	UFUNCTION(BlueprintCallable)
	ABaseAI* GetTargetAI() { return TargetAI; }

	void SetTargetAI(ABaseAI* AI);

protected:
	UPROPERTY()
	ABaseAI* TargetAI;
};
