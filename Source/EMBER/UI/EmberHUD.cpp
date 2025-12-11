// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberHUD.h"
#include "CommonUIExtensions.h"
#include "System/EmberAssetManager.h"
#include "CUserWidget_AIHP.h"
#include "AI/Base/BaseAI.h"
#include "CommonActivatableWidget.h"

void AEmberHUD::BeginPlay()
{
	Super::BeginPlay();

	const FEmberHUDInfo& EmberHUDInfo = UEmberUIData::Get().EmberHUDInfo;

	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player))
		{
			MainScreenWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, EmberHUDInfo.HUDWidgetTag, EmberHUDInfo.HUDWidgetClass);

			GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				if (!MainScreenWidget) return;

				if (UUserWidget* RootUserWidget = Cast<UUserWidget>(MainScreenWidget))
				{
					if (UWidget* Found = RootUserWidget->GetWidgetFromName(TEXT("WBP_HUD_AIHP")))
					{
						if (UCUserWidget_AIHP* AIHPWidget = Cast<UCUserWidget_AIHP>(Found))
						{
							AIHPWidget->SetTargetAI(nullptr);
							UE_LOG(LogTemp, Warning, TEXT("TargetAI is nullptr"));
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("AIHPWidget cast failed"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("AIHPWidget not found"));
					}
				}
			});
		}
	}
}
