#include "Component/StatusComponent.h"

#include "AbilitySystemComponent.h"
#include "EmberPlayerCharacter.h"
#include "GameFlag.h"
#include "GameFramework/Character.h"
#include "GameInfo/GameplayTags.h"
#include "Net/UnrealNetwork.h"

UStatusComponent::UStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	HP = MaxHP;
	Stamina = MaxStamina;
	Temperature = MaxTemperature;
	Energy = MaxEnergy;
	PhysicalStrength = MaxPhysicalStrength;
	Dexterity = MaxDexterity;
	XP = 0.0f;
}

void UStatusComponent::SetMaxHp(float InAmount)
{
	MaxHP += InAmount;
	HP = MaxHP;
}

void UStatusComponent::SetMaxStamina(float InAmount)
{
	MaxStamina += InAmount;
	Stamina = MaxStamina;
}

void UStatusComponent::SetMaxTemperature(float InAmount)
{
	MaxTemperature += InAmount;
	Temperature = MaxTemperature;
}

void UStatusComponent::SetMaxFatigueLevel(float InAmount)
{
	MaxFatigueLevel += InAmount;
}

void UStatusComponent::SetMaxWeight(float InAmount)
{
	MaxWeight += InAmount;
}

void UStatusComponent::SetMaxEnergy(float InAmount)
{
	MaxEnergy += InAmount;
	Energy = MaxEnergy;
}

void UStatusComponent::SetMaxPhysicalStrength(float InAmount)
{
	MaxPhysicalStrength += InAmount;
	PhysicalStrength = MaxPhysicalStrength;
}

void UStatusComponent::SetMaxDexterity(int32 InAmount)
{
	MaxDexterity += InAmount;
	Dexterity = MaxDexterity;
}

void UStatusComponent::SetMaxXp(int32 InAmount)
{
	MaxXP += InAmount;
}

void UStatusComponent::SetMaxLevel(int32 InAmount)
{
	MaxLevel = InAmount;
}

void UStatusComponent::OnRep_Damage()
{
	UE_LOG(LogTemp, Error, L"onrep hp %f", HP);
}

void UStatusComponent::Damage(float InAmount)
{
	HP += (InAmount * -1.0f);
	HP = FMath::Clamp(HP, 0.0f, MaxHP);
}

void UStatusComponent::UseStamina(float InAmount)
{
	Stamina += (InAmount * -1.0f);
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
}

void UStatusComponent::AddHP(float InAmount)
{
	InAmount = FMath::Clamp(InAmount, 0.0f, INT_MAX);
	HP = FMath::Clamp(HP + InAmount, 0.0f, MaxHP);
}

void UStatusComponent::UseTemperature(float InAmount)
{
	Temperature -= InAmount;
	Temperature = FMath::Clamp(Temperature, 0.0f, MaxTemperature);
}

void UStatusComponent::AddTemperature(float InAmount)
{
	Temperature += InAmount;
	Temperature = FMath::Clamp(Temperature, 0.0f, MaxTemperature);
}

void UStatusComponent::AddFatigueLevel(float InAmount)
{
	FatigueLevel += InAmount;
	FatigueLevel = FMath::Clamp(FatigueLevel, 0.0f, MaxFatigueLevel);

	//TODOS �µ� �ִ�ġ�϶� �ɷ�ġ ����
}

void UStatusComponent::AddWeight(float InAmount)
{
	Weight += InAmount;
	Weight = FMath::Clamp(Weight, 0.0f, MaxWeight);

	//TODOS ���� �ִ�ġ�϶� �ɷ�ġ ����
}

void UStatusComponent::UseEnergy(float InAmount)
{
	Energy += (InAmount * -1.0f);
	Energy = FMath::Clamp(Energy, 0.0f, MaxEnergy);

	//TODOS ������ 0�϶� �ɷ�ġ ����
}

void UStatusComponent::AddPhysicalStrength(float InAmount)
{
	PhysicalStrength += InAmount;
	PhysicalStrength = FMath::Clamp(PhysicalStrength, 0.0f, MaxPhysicalStrength);
}

void UStatusComponent::AddDexterity(float InAmount)
{
	Dexterity += InAmount;
	Dexterity = FMath::Clamp(Dexterity, 0.0f, MaxDexterity);
}

void UStatusComponent::AddXp(float InAmount)
{
	XP += InAmount;
	XP = FMath::Clamp(XP, 0.0f, MaxXP);
}

void UStatusComponent::LevelUp()
{
	XP = 0.0f;
	Level++;

	//TODOS �������� �����Ұ� �ۼ�
}

void UStatusComponent::StartDeath()
{
	if (DeathState != EDeathState::NotDead)
	{
		return;
	}

	DeathState = EDeathState::DeathStarted;

	if (OwnerCharacter)
	{
		AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(OwnerCharacter);
		if (UAbilitySystemComponent* EmberASC = EmberCharacter->GetAbilitySystemComponent())
		{
			EmberASC->SetLooseGameplayTagCount(EmberGameplayTags::Status_Death_Dying, 1);
		}
	}

	OnDeathStarted.Broadcast(OwnerCharacter);

	OwnerCharacter->ForceNetUpdate();
}

void UStatusComponent::FinishDeath()
{
	if (DeathState != EDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EDeathState::DeathFinished;

	if (OwnerCharacter)
	{
		AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(OwnerCharacter);
		if (UAbilitySystemComponent* EmberASC = EmberCharacter->GetAbilitySystemComponent())
		{
			EmberASC->SetLooseGameplayTagCount(EmberGameplayTags::Status_Death_Dying, 1);
		}
	}

	OnDeathFinished.Broadcast(OwnerCharacter);

	OwnerCharacter->ForceNetUpdate();
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, HP);
    DOREPLIFETIME(ThisClass, Temperature);
	DOREPLIFETIME(ThisClass, DeathState);
}
