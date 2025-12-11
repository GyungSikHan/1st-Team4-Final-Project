// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "CAI_Weapon.generated.h"

class UC_CharacterMovementComponent;
class UC_StateComponent;
UCLASS()
class EMBER_API ACAI_Weapon : public AActor
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }
	FORCEINLINE FAttackData GetAttackData() { return AttackDatas[Index]; }
public:	
	ACAI_Weapon();

public:
	TArray<FAttackData> GetAttackDatas(){return AttackDatas;}
	UFUNCTION(BlueprintNativeEvent, Category = "InitPlay")
	void SpawnPlay(ACharacter* InOwner); //, const TArray<FAttackData>& InAttackDatas, const TArray<FDamageData>& InDamageDatas);
	virtual void SpawnPlay_Implementation(ACharacter* InOwner);
	virtual void Tick(float InDeltaTime) { }
public:
	virtual void DoAction(int32 AttackIndex);
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	void OnCollision();
	void OffCollision();

protected:
	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachTo(FName InSocketName);

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual int32 GetAttackStack() const { return AttackStack; }
	virtual void ResetAttackStack() { AttackStack = 0; }
	virtual int32 GetMissStack() const { return MissStack; }
	virtual void IncreaseMissStack() { MissStack++; }
	virtual void ResetMissStack() { MissStack = 0; }
	virtual bool GetDidAttack() const { return bDidAttack; }

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Collision")
	TObjectPtr< USceneComponent > Root;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Collision")
	TArray< TObjectPtr< USceneComponent  > > Collisions;
	UPROPERTY(EditAnywhere, Category = "AttackData")
	TArray<FAttackData> AttackDatas;
	UPROPERTY(EditAnywhere, Category = "HitData")
	TArray<FDamageData> HitDatas;

protected:
	class ACharacter* OwnerCharacter;
	bool bInAction;
	bool bBeginAction;

	TObjectPtr<UC_StateComponent> State;
	TObjectPtr<UC_CharacterMovementComponent> Movement;
	int32 Index;
	int32 CurrAttackIndex;
	TArray<ACharacter*> Hitted;

private:
	int32 AttackStack = 0;
	int32 MissStack = 0;
	bool bDidAttack = false;
};


