// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "C_CameraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API UC_CameraComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class AEmberPlayerCharacter;
private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 45.0f;
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 45.0f;

public:
	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

public:	
	UC_CameraComponent();

protected:
	virtual void BeginPlay() override;

public:
	void EnableControlRotation();
	void DisableControlRotation();

private:
	void OnLook(const FInputActionValue& Value);

public:
	virtual void PostInitProperties() override;

private:
	ACharacter* OwnerCharacter;
	bool bFixedCamera;
	bool bControlRotation;
};
