// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "WeaponImpactField.generated.h"

class URadialFalloff;
class URandomVector;
class UUniformVector;
class UOperatorField;
class URadialVector;
class USphereComponent;

UCLASS()
class EMBER_API AWeaponImpactField : public AFieldSystemActor
{
	GENERATED_BODY()

public:
	AWeaponImpactField(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void Init(float InForceMagnitude, float InSphereScaleMultiplier);
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereCollisionComponent;
	
private:
	/** Radial Push 벡터 */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URadialVector> RadialVectorForce;
	
	/** Strain용 거리 기반 감소 필드 */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URadialFalloff> RadialFalloffStrain;

	/** Force용 거리 기반 Falloff */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URadialFalloff> RadialFalloffForce;
	
	/** 임의 방향 벡터 (Torque) */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URandomVector> RandomVector;

	/** 일정 방향 벡터 (Torque) */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUniformVector> UniformVector;

	/** Torque 합성 필드 (Random + Uniform) */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UOperatorField> OperatorFieldTorque;

	/** Force 조합 필드 (Falloff * Vector) */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UOperatorField> OperatorFieldForce;

/* Sphere */
private:
	UPROPERTY(EditDefaultsOnly, Category="Sphere")
	float SphereRadius;
	
	UPROPERTY(EditDefaultsOnly, Category="Sphere")
	float SphereScaleMultiplier;

	UPROPERTY(EditDefaultsOnly, Category="Sphere")
	bool bHideSphereCollision;

/* RadialFalloff */
private:
	UPROPERTY(EditDefaultsOnly, Category="FieldSystemObject")
	float ForceMagnitude;

	UPROPERTY(EditDefaultsOnly, Category="FieldSystemObject")
	float StrainMagnitude;
};
