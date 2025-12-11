// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponImpactField.h"

#include "Components/SphereComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"

AWeaponImpactField::AWeaponImpactField(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = false;

	// Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMeshComponent"));
	MeshComponent->SetHiddenInGame(true);
	MeshComponent->SetupAttachment(GetRootComponent());

	// Colistion
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	SphereCollisionComponent->SetSphereRadius(SphereRadius * SphereScaleMultiplier, true);
	SphereCollisionComponent->SetupAttachment(GetRootComponent());

	// RadialFalloff - Force
	RadialFalloffStrain = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloffStrain"));
	RadialVectorForce = CreateDefaultSubobject<URadialVector>(TEXT("RadialVectorForce"));
	RadialFalloffForce = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloffForce"));
	OperatorFieldForce = CreateDefaultSubobject<UOperatorField>(TEXT("OperatorFieldForce"));
	
	// Uniform Vector - Torque
	UniformVector = CreateDefaultSubobject<UUniformVector>(TEXT("UniformVector"));
	RandomVector = CreateDefaultSubobject<URandomVector>(TEXT("RandomVector"));
	OperatorFieldTorque = CreateDefaultSubobject<UOperatorField>(TEXT("OperatorFieldTorque"));

	ForceMagnitude = 2500.f;
	SphereRadius = 50.f;
	SphereScaleMultiplier = 2.3f;
	StrainMagnitude = 50000.f;
}

void AWeaponImpactField::Init(float InForceMagnitude, float InSphereScaleMultiplier)
{
	ForceMagnitude = InForceMagnitude;
	SphereScaleMultiplier = InSphereScaleMultiplier;

	RadialVectorForce->SetRadialVector(ForceMagnitude, FieldSystemComponent->GetComponentLocation());
	SphereCollisionComponent->SetSphereRadius(SphereRadius * SphereScaleMultiplier, true);
	RadialFalloffStrain->SetRadialFalloff(StrainMagnitude, 0.f, 1.f, 0.f, SphereRadius * SphereScaleMultiplier, SphereCollisionComponent->GetComponentLocation(), EFieldFalloffType::Field_FallOff_None);

	FieldSystemComponent->ResetFieldSystem();
	FieldSystemComponent->AddFieldCommand(true, EFieldPhysicsType::Field_LinearForce, nullptr, OperatorFieldForce);
	FieldSystemComponent->AddFieldCommand(true, EFieldPhysicsType::Field_AngularTorque, nullptr, OperatorFieldTorque);
}

void AWeaponImpactField::BeginPlay()
{
	Super::BeginPlay();

	FVector SphereCollisionLocation = SphereCollisionComponent->GetComponentLocation();

	// RadialFalloff - Strain
	{
		RadialFalloffStrain->SetRadialFalloff(StrainMagnitude, 0.f, 1.f, 0.f, SphereRadius * SphereScaleMultiplier, SphereCollisionLocation, EFieldFalloffType::Field_FallOff_None);
		FieldSystemComponent->AddPersistentField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloffStrain);
	}

	// RadialVector - Force
	{
		float Magnitude = 1.f;
		
		RadialVectorForce->SetRadialVector(ForceMagnitude,  FieldSystemComponent->GetComponentLocation());
		RadialFalloffForce->SetRadialFalloff(Magnitude, 0.f, 1.f, 0.f, SphereRadius, SphereCollisionLocation, EFieldFalloffType::Field_Falloff_Linear);
		OperatorFieldForce->SetOperatorField(Magnitude, RadialFalloffForce, RadialVectorForce, EFieldOperationType::Field_Multiply);
		FieldSystemComponent->AddPersistentField(true, EFieldPhysicsType::Field_LinearForce, nullptr, OperatorFieldForce);
	}

	// Uniform Vector - Torque
	{
		float Magnitude = 1.f;
		
		UniformVector->SetUniformVector(Magnitude, FVector(4.f, 4.f, 10.f));
		RandomVector->SetRandomVector(Magnitude);
		OperatorFieldTorque->SetOperatorField(Magnitude, UniformVector, RandomVector, EFieldOperationType::Field_Multiply);
		FieldSystemComponent->AddPersistentField(true, EFieldPhysicsType::Field_AngularTorque, nullptr, OperatorFieldTorque);
	}
	
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,[this](){ Destroy(); },1.0f,false);
}
