#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EmberTemperature.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEmberTemperature : public UInterface
{
	GENERATED_BODY()
};

class EMBER_API IEmberTemperature
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Temperature")
	void ApplyWarmingEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Temperature")
	void RemoveWarmingEffect();
};