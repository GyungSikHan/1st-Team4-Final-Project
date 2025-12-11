// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmberInteractionInfo.generated.h"

class IEmberInteractable;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FEmberInteractionInfo
{
	GENERATED_BODY()

public:
	FORCEINLINE bool operator==(const FEmberInteractionInfo& Other) const
	{
		return Interactable == Other.Interactable &&
			Title.IdenticalTo(Other.Title) &&
			Content.IdenticalTo(Other.Content);
	}

	FORCEINLINE bool operator!=(const FEmberInteractionInfo& Other) const
	{
		return !operator==(Other);
	}
	
public:
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IEmberInteractable> Interactable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVisible = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityToGrant;
};
