// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimInstance.h"
#include "GameInfo/GameFlag.h"
#include "MontageSystemComponent.generated.h"

USTRUCT()
struct FMontagesData
    : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    EStateType Type;

    UPROPERTY(EditAnywhere)
    class UAnimMontage* Montage;

    UPROPERTY(EditAnywhere)
    float PlayRate = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API UMontageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMontageSystemComponent();
protected:
	virtual void BeginPlay() override;
private:
    UFUNCTION(NetMulticast, Reliable)
    void MulticastPlayMontage(UAnimMontage* Montage,float PlayRate = 1.f,FName SectionName = NAME_None);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerPlayMontage(UAnimMontage* Montage, float PlayRate = 1.f, FName SectionName = NAME_None);
private:
    UPROPERTY()
    USkeletalMeshComponent* MeshComp;
    UPROPERTY()
    UAnimInstance* AnimInstance;
    UPROPERTY(EditAnywhere, Category = "DataTable")
    UDataTable* DataTable;
    
public:
    void PlayMontage(UAnimMontage* Montage, float PlayRate = 1.f,FName SectionName = NAME_None);
    void PlayMontage(EStateType InType);
private:
    FMontagesData* Datas[(int32)EStateType::Max];
    ACharacter* OwnerCharacter;
};
