#include "Interaction/RespawnSubsystem.h"

#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"

void URespawnSubsystem::SetNewRespawnTransform(const FTransform& NewTransform)
{
	CurrentRespawnTransform = NewTransform;
	bHasCustomRespawnLocation = true;
	UE_LOG(LogTemp, Log, TEXT("New respawn point set to: %s"), *NewTransform.GetLocation().ToString());
}

FTransform URespawnSubsystem::GetRespawnTransform() const
{
	if (bHasCustomRespawnLocation)
	{	
		return CurrentRespawnTransform;
	}
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	FVector Location{};
	FRotator Rotation{};
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	if (gm != nullptr)
	{
		AActor* st = gm->ChoosePlayerStart(pc);
		if (st != nullptr)
		{
			Location = st->GetActorLocation();
			Rotation = st->GetActorRotation();

			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("%s / %s"), *Location.ToString(), *Rotation.ToString()));
		}
	}
	return FTransform(Rotation, Location);
}