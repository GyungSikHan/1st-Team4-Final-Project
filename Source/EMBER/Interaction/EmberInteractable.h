// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmberInteractionInfo.h"
#include "UObject/Interface.h"
#include "EmberInteractable.generated.h"

class UAbilitySystemComponent;
struct FEmberInteractionQuery;

UINTERFACE(MinimalAPI, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class UEmberInteractable : public UInterface
{
	GENERATED_BODY()
};

class EMBER_API IEmberInteractable
{
	GENERATED_BODY()

public:
	virtual FEmberInteractionInfo GetPreInteractionInfo(const FEmberInteractionQuery& InteractionQuery) const { return FEmberInteractionInfo(); }
	
	FEmberInteractionInfo GatherPostInteractionInfo(const FEmberInteractionQuery& InteractionQuery, TScriptInterface<IEmberInteractable> Interactable)
	{
		FEmberInteractionInfo InteractionInfo = GetPreInteractionInfo(InteractionQuery);
		
		// TODO : 능력치에 따른 상호작용 정보 설정
		InteractionInfo.Interactable = Interactable;
		return InteractionInfo;
	}
	
	UFUNCTION(BlueprintCallable)
	virtual UMeshComponent* GetMeshComponent() const { return nullptr; }
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanInteraction(UAbilitySystemComponent* ASC) const { return true; }
};
