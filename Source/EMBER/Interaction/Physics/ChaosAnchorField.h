// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "ChaosAnchorField.generated.h"

class UUniformInteger;
class UCullingField;
class UBoxFalloff;

UCLASS()
class EMBER_API AChaosAnchorField : public AFieldSystemActor
{
	GENERATED_BODY()

public:
	AChaosAnchorField(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostInitializeComponents() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UUniformInteger> UniformInteger;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCullingField> CullingField;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxFalloff> BoxFalloff;

private:
	UPROPERTY(EditDefaultsOnly, Category = "ChaosAnchorField")
	float FieldMagnitude = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "ChaosAnchorField")
	float MinRange = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "ChaosAnchorField")
	float MaxRange = 0.f;
};
