// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment_Equipable.h"
#include "ItemFragment_Equipable_Crafting.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UItemFragment_Equipable_Crafting : public UItemFragment_Equipable
{
	GENERATED_BODY()

public:
	UItemFragment_Equipable_Crafting(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY(EditDefaultsOnly)
	ECraftingType CraftingType;
};
