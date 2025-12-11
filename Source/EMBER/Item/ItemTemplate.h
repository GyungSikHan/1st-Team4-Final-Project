// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "ItemTemplate.generated.h"

class UItemInstance;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class UItemFragment : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnInstanceCreated(UItemInstance* Instance) const { }
};

UCLASS(Abstract, Blueprintable, Const)
class EMBER_API UItemTemplate : public UObject
{
	GENERATED_BODY()

public:
	UItemTemplate(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());	

public:
	UFUNCTION(BlueprintCallable)
	const UItemFragment* FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const;
	
	template <typename FragmentClass>
	FragmentClass* FindFragmentByClass() const
	{
		return (FragmentClass*)FindFragmentByClass(FragmentClass::StaticClass());
	}
	
public:
	UPROPERTY(EditDefaultsOnly)
	FIntPoint SlotCount = FIntPoint::ZeroValue;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackCount = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> IconTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> PickupableMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UItemFragment>> Fragments;
};
