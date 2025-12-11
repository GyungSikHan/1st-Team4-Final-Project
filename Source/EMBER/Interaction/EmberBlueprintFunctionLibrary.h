#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EmberBlueprintFunctionLibrary.generated.h"

class URespawnSubsystem;

UCLASS()
class EMBER_API UEmberBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Ember|Subsystems")
	static URespawnSubsystem* GetRespawnSubsystem();
};