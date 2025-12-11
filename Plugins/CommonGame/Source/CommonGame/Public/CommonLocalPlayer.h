// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Engine/LocalPlayer.h"

#include "CommonLocalPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController);

UCLASS()
class COMMONGAME_API UCommonLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	UCommonLocalPlayer();
	
	FPlayerControllerSetDelegate OnPlayerControllerSet;
};
