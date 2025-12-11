// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"

#include "EmberActionHintInfo.generated.h"

USTRUCT(BlueprintType)
struct FEmberActionHintInfo
{
	GENERATED_BODY()

public:
	FORCEINLINE bool operator==(const FGameplayTag& OtherAbilityTag) const
	{
		return AbilityTag == OtherAbilityTag;
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Ability"))
	FGameplayTag AbilityTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Content;
};
