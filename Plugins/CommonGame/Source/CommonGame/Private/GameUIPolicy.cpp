// Fill out your copyright notice in the Description page of Project Settings.

#include "GameUIPolicy.h"

#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"

UGameUIPolicy::UGameUIPolicy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameUIPolicy::AddLayoutToViewport(FLocalPlayerContext LocalPlayer, UPrimaryGameLayout* Layout)
{
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(/* ZOrder */1000);
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		Layout->RemoveFromParent();
	}
}

void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass();
		if (LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract))
		{
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayout.LocalPlayer = LocalPlayer;
			RootViewportLayout.RootLayout = NewLayoutObject;
			RootViewportLayout.bAddedToViewport = true;

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
	{
		NotifyPlayerRemoved(LocalPlayer);
		if (RootViewportLayout.RootLayout)
		{
			AddLayoutToViewport(LocalPlayer, RootViewportLayout.RootLayout);
			RootViewportLayout.LocalPlayer = LocalPlayer;
			RootViewportLayout.bAddedToViewport = true;
		}
		else
		{
			CreateLayoutWidget(LocalPlayer);
		}
	});

	if (RootViewportLayout.RootLayout)
	{
		AddLayoutToViewport(LocalPlayer, RootViewportLayout.RootLayout);
		RootViewportLayout.LocalPlayer = LocalPlayer;
		RootViewportLayout.bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (RootViewportLayout.RootLayout)
	{
		RemoveLayoutFromViewport(LocalPlayer, RootViewportLayout.RootLayout);
		RootViewportLayout.LocalPlayer = nullptr;
		RootViewportLayout.bAddedToViewport = false;
	}
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	RootViewportLayout.RootLayout = nullptr;
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass()
{
	return LayoutClass.LoadSynchronous();
}