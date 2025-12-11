#pragma once
#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFlag.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "GAmeData.generated.h"

USTRUCT(BlueprintType)
struct FEffectData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    EEffectType EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (EditCondition = "EffectType == EEffectType::Niagara", EditConditionHides))
    TObjectPtr<UNiagaraSystem> NiagaraSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (EditCondition = "EffectType == EEffectType::Particle", EditConditionHides))
    TObjectPtr<UParticleSystem> ParticleSystem;
};

USTRUCT(BlueprintType)
struct FSound2D
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USoundWave> Sound; // 2D 사운드 에셋

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Volume; // 볼륨

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Pitch; // 피치
};

USTRUCT(BlueprintType)
struct FParryData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;	//공격속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEffectData HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSound2D HitSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TObjectPtr<UAnimMontage>> Montages;
};

USTRUCT(BlueprintType)
struct FEquipment
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSound2D EquipSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> HolsterMontage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> DrawMontage;
};

USTRUCT(BlueprintType)
struct FDamageData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEffectData HitEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSound2D HitSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> Montages;

public:
    void SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther);
};

USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayRate = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEffectData AttackEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSound2D AttackSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> Montages;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MontageIndex;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanMove;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanFixedCamera;

public:
    void DoAction(ACharacter* InOwner);
};

USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
    GENERATED_BODY()
public:
    FDamageData* DamageData;
};


USTRUCT(BlueprintType)
struct FCharacterStatus
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentHP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentStamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxStamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentTemperature;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxTemperature;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentFatigueLevel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxFatigueLevel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentWeight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWeight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentEnergy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxEnergy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentPhysicalStrength;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxPhysicalStrength;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDexterity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDexterity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentXP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxXP;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;
public:
    float GetCurrentHP() const;
    float GetMaxHP() const;
    float GetCurrentStamina() const;
    float GetMaxStamina() const;
    float GetCurrentTemperature() const;
    float GetMaxTemperature() const;
    float GetCurrentFatigueLevel() const;
    float GetMaxFatigueLevel() const;
    float GetCurrentWeight() const;
    float GetMaxWeight() const;
    float GetCurrentEnergy() const;
    float GetMaxEnergy() const;
    float GetCurrentPhysicalStrength() const;
    float GetMaxPhysicalStrength() const;
    float GetCurrentDexterity() const;
    float GetMaxDexterity() const;
    float GetCurrentXP() const;
    float GetMaxXP() const;
    int32 GetLevel() const;
public:
    void SetCurrentHP(const float Amount);
    void SetMaxHP(const float Amount);
    void SetCurrentStamina(const float Amount);
    void SetMaxStamina(const float Amount);
    void SetCurrentTemperature(const float Amount);
    void SetMaxTemperature(const float Amount);
    void SetCurrentFatigueLevel(const float Amount);
    void SetMaxFatigueLevel(const float Amount);
    void SetCurrentWeight(const float Amount);
    void SetMaxWeight(const float Amount);
    void SetCurrentEnergy(const float Amount);
    void SetMaxEnergy(const float Amount);
    void SetCurrentPhysicalStrength(const float Amount);
    void SetMaxPhysicalStrength(const float Amount);
    void SetCurrentDexterity(const float Amount);
    void SetMaxDexterity(const float Amount);
    void SetCurrentXP(const float Amount);
    void SetMaxXP(const float Amount);
    void LevelUp(const int32 Amount);
};

USTRUCT()
struct FDamagesData
{
    GENERATED_BODY()
    UPROPERTY()
    float Power;
    UPROPERTY()
    class ACharacter* Character;
    UPROPERTY()
    class AActor* Causer;
    UPROPERTY()
    UAnimMontage* Montage;
    UPROPERTY()
    float PlayRate;
};