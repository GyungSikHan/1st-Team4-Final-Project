#include "AI/AIWeapon/DragonSpitProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFrameWork/Character.h"
#include "Kismet/GameplayStatics.h"

ADragonSpitProjectile::ADragonSpitProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SpitCollision = CreateDefaultSubobject<USphereComponent>(FName("SpitCollision"));
	SpitCollision->InitSphereRadius(15.f);
	SpitCollision->SetCollisionProfileName("Custom");
	SpitCollision->IgnoreActorWhenMoving(this, true);
	SetRootComponent(SpitCollision);

	SpitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpitEffect"));
	SpitEffect->SetupAttachment(SpitCollision);
	

	SpitMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("SpitMovement"));
	SpitMovement->InitialSpeed = 3000.f;
	SpitMovement->MaxSpeed = 3000.f;
	SpitMovement->bRotationFollowsVelocity = true;
	SpitMovement->bShouldBounce = false;

	InitialLifeSpan = 3.f;
}

void ADragonSpitProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (SpitNiagara == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Dragon.cpp, niagara is null");
	}
	else
	{
		SpitEffect->SetAsset(SpitNiagara);
		SpitEffect->SetAutoActivate(false);
	}
	SpitCollision->OnComponentHit.AddDynamic(this, &ADragonSpitProjectile::OnHit);
}

void ADragonSpitProjectile::SetTargetActor(AActor* Target)
{
	TargetActor = Target;
	
	if (!TargetActor || !SpitMovement) return;
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("TargetActor: %s"), *GetNameSafe(TargetActor)));

	FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	SpitMovement->Velocity = Direction * SpitMovement->InitialSpeed;

	if (SpitNiagara && SpitEffect)
	{
		SpitEffect->SetAsset(SpitNiagara);
		SpitEffect->Activate();
	}
}

void ADragonSpitProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
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
		if (ACharacter* Target = Cast<ACharacter>(OtherActor))
		{
			UGameplayStatics::ApplyDamage(
				Target,
				Damage,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
				);
		}
	}
	
	Destroy();
}


