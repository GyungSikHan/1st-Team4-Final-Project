// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberWorldInteractable.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"

AEmberWorldInteractable::AEmberWorldInteractable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("Interactable"));
	MeshComponent->SetCanEverAffectNavigation(true);
	MeshComponent->SetupAttachment(GetRootComponent());

	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	InteractionCollision->SetCollisionProfileName(TEXT("Interactable"));
	InteractionCollision->SetupAttachment(GetRootComponent());
	
	InteractionInfo.bVisible = false;
}

void AEmberWorldInteractable::OnInteractActiveStarted(AActor* Interactor)
{
	if (IsValid(Interactor) == false)
		return;

	if (HasAuthority())
	{
		CachedInteractors = Interactor;
	}
}

void AEmberWorldInteractable::OnInteractionSuccess(AActor* Interactor)
{
	if (IsValid(Interactor) == false)
		return;
}

FEmberInteractionInfo AEmberWorldInteractable::GetPreInteractionInfo( const FEmberInteractionQuery& InteractionQuery) const
{
	return InteractionInfo;
}

UMeshComponent* AEmberWorldInteractable::GetMeshComponent() const
{
	return MeshComponent;
}

bool AEmberWorldInteractable::CanInteraction(UAbilitySystemComponent* ASC) const
{
	if (ASC == nullptr)
		return false;
	
	return ASC->HasAllMatchingGameplayTags(RequiredTags);
}
