// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interaction/EmberInteractable.h"
#include "EmberWorldInteractable.generated.h"

class UAbilitySystemComponent;
class USphereComponent;

UCLASS(Abstract, BlueprintType, Blueprintable)
class EMBER_API AEmberWorldInteractable : public AActor, public IEmberInteractable
{
	GENERATED_BODY()

public:
	AEmberWorldInteractable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION()
	virtual void OnInteractActiveStarted(AActor* Interactor);

	UFUNCTION(BlueprintCallable)
	virtual void OnInteractionSuccess(AActor* Interactor);
	
public:
	virtual FEmberInteractionInfo GetPreInteractionInfo(const FEmberInteractionQuery& InteractionQuery) const override;
	virtual UMeshComponent* GetMeshComponent() const override;
	virtual bool CanInteraction(UAbilitySystemComponent* ASC) const;

protected:
	UPROPERTY(EditAnywhere, Category="Info")
	FEmberInteractionInfo InteractionInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> InteractionCollision;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer RequiredTags;

	UPROPERTY()
	TWeakObjectPtr<AActor> CachedInteractors;
};
