// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAI.h"
#include "GameFlag.h"
#include "GameFramework/Character.h"
#include "HumanAIBase.generated.h"

class UBehaviorTree;
class UC_StateComponent;

UCLASS()
class EMBER_API AHumanAIBase : public ABaseAI
{
	GENERATED_BODY()
//public:
//	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

public:
	// Sets default values for this character's properties
	AHumanAIBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//UFUNCTION()
	//void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Stat")
	//TObjectPtr<UC_StateComponent> StateComponent;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Status")
	//TObjectPtr<class UStatusComponent> StatusComponent;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move")
	//TObjectPtr<class UC_CharacterMovementComponent> MoveComponent;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Montage")
	//TObjectPtr<class UMontageSystemComponent> MontageComponent;
	/*UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AI|Behavior")
	TObjectPtr< class UCBehaviorTreeComponent> Behavior;*/
	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//TObjectPtr<UBehaviorTree> BehaviorTree;
	//UPROPERTY(EditAnywhere)
	//float MaxHP = 100.0f;
};
