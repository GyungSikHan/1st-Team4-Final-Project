#include "AI/AIWeapon/DragonMeteorProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ADragonMeteorProjectile::ADragonMeteorProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MeteorCollision = CreateDefaultSubobject<USphereComponent>("MeteorCollision");
	MeteorCollision->SetCollisionProfileName("Custom");
	MeteorCollision->SetNotifyRigidBodyCollision(true);
	MeteorCollision->InitSphereRadius(50.0f);
	SetRootComponent(MeteorCollision);

	MeteorEffect = CreateDefaultSubobject<UNiagaraComponent>("MeteorEffect");
	MeteorEffect->SetupAttachment(MeteorCollision);
	MeteorEffect->SetAutoActivate(false);

	MeteorDecal = CreateDefaultSubobject<UDecalComponent>("MeteorDecal");
	MeteorDecal->DecalSize = FVector(100.f, 1.f, 1.f);
	MeteorDecal->SetupAttachment(RootComponent);
	MeteorDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	MeteorDecal->SetRelativeLocation(FVector(0, 0, -MeteorCollision->GetUnscaledSphereRadius()));

	InitialLifeSpan = 5.f;
}

void ADragonMeteorProjectile::InitializeMeteor(UNiagaraSystem* InEffect, float InDamage, float InCollisionRadius)
{
	Damage = InDamage;

	if (InEffect && MeteorEffect)
	{
		MeteorEffect->SetAsset(InEffect);
		MeteorEffect->Activate();
	}

	if (MeteorCollision)
	{
		MeteorCollision->SetSphereRadius(InCollisionRadius);
	}

	TargetDecalSize = InCollisionRadius * 2.f;
	CurrentDecalSize = 10.f;
	
	if (MeteorDecal)
	{
		MeteorDecal->DecalSize = FVector(CurrentDecalSize, 1.f, 1.f);
	}

	FVector GroundLocation = GetActorLocation();
	GroundLocation.Z = 0.f;
	GroundZ = GroundLocation.Z;

	if (DecalMaterial)
	{
		MeteorDecal->SetDecalMaterial(DecalMaterial);
	}
}

void ADragonMeteorProjectile::BeginPlay()
{
	Super::BeginPlay();
	MeteorCollision->OnComponentHit.AddDynamic(this, &ADragonMeteorProjectile::OnHit);
}

void ADragonMeteorProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MeteorDecal && CurrentDecalSize < TargetDecalSize)
	{
		CurrentDecalSize += DecalGrowthSpeed * DeltaTime;
		CurrentDecalSize = FMath::Min(CurrentDecalSize, TargetDecalSize);

		MeteorDecal->DecalSize = FVector(CurrentDecalSize, 1.f, 1.f);
	}
}

void ADragonMeteorProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ImpactNiagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactNiagara,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
			);
	}
	
	if (OtherActor && OtherActor != GetOwner())
	{
		if (ACharacter* HitChar = Cast<ACharacter>(OtherActor))
		{
			UGameplayStatics::ApplyDamage(HitChar, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		}
	}

	if (MeteorEffect)
	{
		MeteorEffect->SetAutoDestroy(true);
		MeteorEffect->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		MeteorEffect = nullptr;
	}

	if (MeteorDecal)
	{
		MeteorDecal->DestroyComponent();
	}

	SetLifeSpan(2.5f);
}



