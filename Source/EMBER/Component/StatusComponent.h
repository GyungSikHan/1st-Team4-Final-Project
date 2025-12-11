#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathEvent, AActor*, OwningActor);

UCLASS(ClassGroup = (Custom),meta = (BlueprintSpawnableComponent))
class EMBER_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Status")
	FORCEINLINE float GetMaxHp() { return MaxHP; }
	UFUNCTION(BlueprintCallable, Category="Status")
	FORCEINLINE float GetHp() { return HP; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina() { return Stamina; }
	FORCEINLINE float GetMaxTemperature() { return MaxTemperature; }
	FORCEINLINE float GetTemperature() { return Temperature; }
	FORCEINLINE float GetMaxFatigueLevel() { return MaxFatigueLevel; }
	FORCEINLINE float GetFatigueLevel() { return FatigueLevel; }
	FORCEINLINE float GetMaxWeight() { return MaxWeight; }
	FORCEINLINE float GetWeight() { return Weight; }
	FORCEINLINE float GetMaxEnergy() { return MaxEnergy; }
	FORCEINLINE float GetEnergy() { return Energy; }
	FORCEINLINE float GetMaxPhysicalStrength() { return MaxPhysicalStrength; }
	FORCEINLINE float GetPhysicalStrength() { return PhysicalStrength; }
	FORCEINLINE float GetMaxDexterity() { return MaxDexterity; }
	FORCEINLINE float GetDexterity() { return Dexterity; }
	FORCEINLINE float GetMaxXP() { return MaxXP; }
	FORCEINLINE float GetXP() { return XP; }
	FORCEINLINE float GetMaxLevel() { return MaxLevel; }
	FORCEINLINE float GetLevel() { return Level; }

public:
	UStatusComponent();
	
protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxHp(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxStamina(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxTemperature(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxFatigueLevel(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxWeight(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxEnergy(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxPhysicalStrength(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxDexterity(int32 InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxXp(int32 InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void SetMaxLevel(int32 InAmount);

	UFUNCTION(BlueprintCallable, Category="Status")
	void OnRep_Damage();
	UFUNCTION(BlueprintCallable, Category="Status")
	void Damage(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void UseStamina(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void AddHP(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void AddTemperature(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void UseTemperature(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void AddFatigueLevel(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void AddWeight(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void UseEnergy(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void AddPhysicalStrength(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void AddDexterity(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void AddXp(float InAmount);
	UFUNCTION(BlueprintCallable, Category="Status")
	void LevelUp();

/* Death */
public:
	virtual void StartDeath();
	virtual void FinishDeath();

	UFUNCTION()
	virtual void OnRep_DeathState(EDeathState OldDeathState);
	
	EDeathState GetDeathState() { return DeathState; }
	
	UPROPERTY(BlueprintAssignable)
	FDeathEvent OnDeathStarted;

	UPROPERTY(BlueprintAssignable)
	FDeathEvent OnDeathFinished;

	UPROPERTY(ReplicatedUsing = OnRep_DeathState)
	EDeathState DeathState;
	
private:
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxHP = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxStamina = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxTemperature = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxFatigueLevel = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxWeight = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxEnergy = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxPhysicalStrength = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxDexterity = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxXP = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Status")
	int32 MaxLevel = 30;

private:
	UPROPERTY(ReplicatedUsing = "OnRep_Damage")
	float HP;
	float Stamina;
    UPROPERTY(Replicated)
	float Temperature;
	float FatigueLevel = 0.0f;
	float Weight = 0.0f;
	float Energy;
	float PhysicalStrength;
	float Dexterity;
	float XP = 0.0f;

	ACharacter* OwnerCharacter;

	int32 Level;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};

inline void UStatusComponent::OnRep_DeathState(EDeathState OldDeathState)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_DeathState"));
}