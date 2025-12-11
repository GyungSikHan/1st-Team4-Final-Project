// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EAIWeaponType : uint8
{
	Bite = 0, Max
};

class ACAI_Weapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API UCAIWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCAIWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void DoAction(int32 AttackIndex);

	ACAI_Weapon* GetDoAction();

	void DestroyWeapon();

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TSubclassOf<ACAI_Weapon>> WeaponDatas;

private:
	TObjectPtr<ACharacter> OwnerCharacter;
public:
	TArray<TObjectPtr<ACAI_Weapon>> Weapons;
};
