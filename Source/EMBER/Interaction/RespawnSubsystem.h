#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RespawnSubsystem.generated.h"

UCLASS()
class EMBER_API URespawnSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void SetNewRespawnTransform(const FTransform& NewTransform);

	UFUNCTION(BlueprintPure, Category = "Respawn")
	FTransform GetRespawnTransform() const;

private:
	bool bHasCustomRespawnLocation = false;

	FTransform CurrentRespawnTransform;
};