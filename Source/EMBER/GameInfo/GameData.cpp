#include "GameData.h"

#include "C_CameraComponent.h"
#include "C_CharacterMovementComponent.h"
#include "MontageSystemComponent.h"
#include "AIWeapon/CAI_Weapon.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

//FDamageData
void FDamageData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
    if(InAttacker->HasAuthority() == false)
        return;
    FActionDamageEvent e;
    e.DamageData = this;
    //UGameplayStatics::ApplyDamage(InOther, Damage, InAttacker->GetController(), InAttackCauser, damageType);
    InOther->TakeDamage(Damage, e, InAttacker->GetController(), InAttackCauser);
}
/////////////////////////////////////////////////////////////////////////////////////////////
//FAttackData
void FAttackData::DoAction(ACharacter* InOwner)
{
    UC_CharacterMovementComponent* movement = Cast<UC_CharacterMovementComponent>(InOwner->GetComponentByClass(UC_CharacterMovementComponent::StaticClass()));
    if(movement != nullptr)
    {
        if (bCanMove == true)
            movement->EnableMove();
        else
            movement->DisableMove();
    }
    UC_CameraComponent* camera = Cast<UC_CameraComponent>(InOwner->GetComponentByClass(UC_CameraComponent::StaticClass()));
	if(camera != nullptr)
	{
        if (bCanFixedCamera == true)
            camera->EnableFixedCamera();
        else
            camera->DisableFixedCamera();
	}
    UMontageSystemComponent* montage = Cast<UMontageSystemComponent>(InOwner->GetComponentByClass(UMontageSystemComponent::StaticClass()));
    if(montage == nullptr || Montages == nullptr)
        return;
    
    montage->PlayMontage(Montages, PlayRate);
    /*if (Montages[0] != nullptr)
    {
	    
        InOwner->PlayAnimMontage(Montages[0], PlayRate);
    }*/
}

////////////////////////////////////////////////////////////////////////////

float FCharacterStatus::GetCurrentHP() const
{
    return CurrentHP;
}

float FCharacterStatus::GetMaxHP() const
{
    return MaxHP;
}

float FCharacterStatus::GetCurrentStamina() const
{
    return CurrentStamina;
}

float FCharacterStatus::GetMaxStamina() const
{
    return MaxStamina;
}

float FCharacterStatus::GetCurrentTemperature() const
{
    return CurrentTemperature;
}

float FCharacterStatus::GetMaxTemperature() const
{
    return MaxTemperature;
}

float FCharacterStatus::GetCurrentFatigueLevel() const
{
    return CurrentFatigueLevel;
}

float FCharacterStatus::GetMaxFatigueLevel() const
{
    return MaxFatigueLevel;
}

float FCharacterStatus::GetCurrentWeight() const
{
    return CurrentWeight;
}

float FCharacterStatus::GetMaxWeight() const
{
    return MaxWeight;
}

float FCharacterStatus::GetCurrentEnergy() const
{
    return CurrentEnergy;
}

float FCharacterStatus::GetMaxEnergy() const
{
    return MaxEnergy;
}

float FCharacterStatus::GetCurrentPhysicalStrength() const
{
    return CurrentPhysicalStrength;
}

float FCharacterStatus::GetMaxPhysicalStrength() const
{
    return MaxPhysicalStrength;
}

float FCharacterStatus::GetCurrentDexterity() const
{
    return CurrentDexterity;
}

float FCharacterStatus::GetMaxDexterity() const
{
    return MaxDexterity;
}

float FCharacterStatus::GetCurrentXP() const
{
    return CurrentXP;
}

float FCharacterStatus::GetMaxXP() const
{
    return MaxXP;
}

int32 FCharacterStatus::GetLevel() const
{
    return Level;
}

void FCharacterStatus::SetCurrentHP(const float Amount)
{
    CurrentHP = Amount;
}

void FCharacterStatus::SetMaxHP(const float Amount)
{
    MaxHP = Amount;
}

void FCharacterStatus::SetCurrentStamina(const float Amount)
{
    CurrentStamina = Amount;
}

void FCharacterStatus::SetMaxStamina(const float Amount)
{
    MaxStamina = Amount;
}

void FCharacterStatus::SetCurrentTemperature(const float Amount)
{
    CurrentTemperature = Amount;
}

void FCharacterStatus::SetMaxTemperature(const float Amount)
{
    MaxTemperature = Amount;
}

void FCharacterStatus::SetCurrentFatigueLevel(const float Amount)
{
    CurrentFatigueLevel = Amount;
}

void FCharacterStatus::SetMaxFatigueLevel(const float Amount)
{
    MaxFatigueLevel = Amount;
}

void FCharacterStatus::SetCurrentWeight(const float Amount)
{
    CurrentWeight = Amount;
}

void FCharacterStatus::SetMaxWeight(const float Amount)
{
    MaxWeight = Amount;
}

void FCharacterStatus::SetCurrentEnergy(const float Amount)
{
    CurrentEnergy = Amount;
}

void FCharacterStatus::SetMaxEnergy(const float Amount)
{
    MaxEnergy = Amount;
}

void FCharacterStatus::SetCurrentPhysicalStrength(const float Amount)
{
    CurrentPhysicalStrength = Amount;
}

void FCharacterStatus::SetMaxPhysicalStrength(const float Amount)
{
    MaxPhysicalStrength = Amount;
}

void FCharacterStatus::SetCurrentDexterity(const float Amount)
{
    CurrentDexterity = Amount;
}

void FCharacterStatus::SetMaxDexterity(const float Amount)
{
    MaxDexterity = Amount;
}

void FCharacterStatus::SetCurrentXP(const float Amount)
{
    CurrentXP = Amount;
}

void FCharacterStatus::SetMaxXP(const float Amount)
{
    MaxXP = Amount;
}

void FCharacterStatus::LevelUp(const int32 Amount)
{
    Level = Amount;
}
