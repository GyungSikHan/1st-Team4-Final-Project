// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryGameLayout.h"

#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

UPrimaryGameLayout::UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

/*static*/UPrimaryGameLayout* UPrimaryGameLayout::GetPrimaryGameLayout(APlayerController* PlayerController)
{
	return PlayerController ? GetPrimaryGameLayout(Cast<UCommonLocalPlayer>(PlayerController->Player)) : nullptr;
}

/*static*/UPrimaryGameLayout* UPrimaryGameLayout::GetPrimaryGameLayout(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer)
	{
		const UCommonLocalPlayer* CommonLocalPlayer = CastChecked<UCommonLocalPlayer>(LocalPlayer);
		if (const UGameInstance* GameInstance = CommonLocalPlayer->GetGameInstance())
		{
			if (UGameUIManagerSubsystem* UIManager = GameInstance->GetSubsystem<UGameUIManagerSubsystem>())
			{
				if (const UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
				{
					if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout())
					{
						return RootLayout;
					}
				}
			}
		}
	}

	return nullptr;
}

void UPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	if (!IsDesignTime())
	{
		LayerWidget->SetTransitionDuration(0.0);
		Layers.Add(LayerTag, LayerWidget);
	}
}

UCommonActivatableWidgetContainerBase* UPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerName)
{
	return Layers.FindRef(LayerName);
}
