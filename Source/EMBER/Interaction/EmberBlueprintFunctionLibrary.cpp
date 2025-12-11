#include "Interaction/EmberBlueprintFunctionLibrary.h"
#include "Interaction/RespawnSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

URespawnSubsystem* UEmberBlueprintFunctionLibrary::GetRespawnSubsystem()
{
	if (GEngine)
	{
		FWorldContext* WorldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		if (WorldContext && WorldContext->World())
		{
			if (UGameInstance* GameInstance = WorldContext->World()->GetGameInstance())
			{
				return GameInstance->GetSubsystem<URespawnSubsystem>();
			}
		}
	}
	return nullptr;
}