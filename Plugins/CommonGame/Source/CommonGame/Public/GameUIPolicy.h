// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameUIPolicy.generated.h"

class UGameUIManagerSubsystem;
class UCommonLocalPlayer;
class UPrimaryGameLayout;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
	
	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}
	
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;
	
	UPROPERTY(Transient)
	bool bAddedToViewport = false;
};

UCLASS(Abstract, Blueprintable)
class COMMONGAME_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()
	
public:
	UGameUIPolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPrimaryGameLayout* GetRootLayout() const { return RootViewportLayout.RootLayout; }
	
protected:
	void AddLayoutToViewport(FLocalPlayerContext LocalPlayer, UPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	
	void CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer);
	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass();

private:
	friend UGameUIManagerSubsystem;
	
	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);
	
private:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;
	
	UPROPERTY()
	FRootViewportLayoutInfo RootViewportLayout;
};
