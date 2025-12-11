// Fill out your copyright notice in the Description page of Project Settings.


#include "CAI_Weapon.h"

#include "C_CharacterMovementComponent.h"
#include "C_StateComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ACAI_Weapon::ACAI_Weapon()
{
	Root = CreateDefaultSubobject<USceneComponent>(L"Root");
}

void ACAI_Weapon::SpawnPlay_Implementation(ACharacter* InOwner)
{
	UE_LOG(LogTemp, Warning, L"Weapon");
	OwnerCharacter = Cast<ACharacter>(InOwner);
	if(OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Owner is null");
		return;
	}
	State = Cast<UC_StateComponent>(OwnerCharacter->GetComponentByClass(UC_StateComponent::StaticClass()));
	Movement = Cast<UC_CharacterMovementComponent>(OwnerCharacter->GetComponentByClass(UC_CharacterMovementComponent::StaticClass()));
	if(State == nullptr)
		UE_LOG(LogTemp, Error, L"State is null");

	if (Movement == nullptr)
		UE_LOG(LogTemp, Error, L"Movement is null");

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);
		USphereComponent* sphere = Cast<USphereComponent>(child);
		if(shape != nullptr)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this,&ACAI_Weapon::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAI_Weapon::OnComponentEndOverlap);
			Collisions.Add(child);
		}
		else if(sphere != nullptr)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAI_Weapon::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAI_Weapon::OnComponentEndOverlap);
			Collisions.Add(child);
		}
	}
	OffCollision();
}

void ACAI_Weapon::DoAction(int32 AttackIndex)
{
	bInAction = true;
	State->SetActionMode();
}

void ACAI_Weapon::Begin_DoAction()
{
	bBeginAction = true;
}

void ACAI_Weapon::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;
		
	State->SetIdleMode();
	Movement->EnableMove();
}

void ACAI_Weapon::OnCollision()
{
	// UE_LOG(LogTemp, Warning, L"collision");
	for (USceneComponent* collision : Collisions)
	{
		UPrimitiveComponent* coll = Cast<UPrimitiveComponent >(collision);
		if (coll != nullptr)
			coll->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAI_Weapon::OffCollision()
{
	for (USceneComponent* collision : Collisions)
	{
		UPrimitiveComponent* coll = Cast<UPrimitiveComponent >(collision);
		if (coll != nullptr)
			coll->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACAI_Weapon::AttachTo(FName InSocketName)
{
	if(OwnerCharacter == nullptr)
		return;
	for (USceneComponent* collision : Collisions)
	{
		if(collision->GetName() == InSocketName.ToString())
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
	}
}

void ACAI_Weapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr)
		return;
	ACharacter* other = Cast<ACharacter>(OtherActor);
	if (other == nullptr)
		return;
	if (OwnerCharacter == other)
		return;
	if(OwnerCharacter->GetClass() == other->GetClass())
		return;

	for (ACharacter* hitted : Hitted)
		if (hitted == other)
			return;
	
	Hitted.AddUnique(other);
	AttackStack++;
	bDidAttack = true;
	HitDatas[CurrAttackIndex].SendDamage(OwnerCharacter, this, other);
}

void ACAI_Weapon::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
		return;
	if (OwnerCharacter == OtherActor)
		return;
	if (OwnerCharacter->GetClass() == OtherActor->GetClass())
		return;

	Hitted.Empty();
}