#include "AI/AIWeapon/DragonBreathActor.h"
#include "AI/Boss/Dragon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

ADragonBreathActor::ADragonBreathActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADragonBreathActor::BeginPlay()
{
	Super::BeginPlay();

	Dragon = Cast<ADragon>(UGameplayStatics::GetActorOfClass(GetWorld(), ADragon::StaticClass()));

	if (Dragon && BreathEffect)
	{
		BreathEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			BreathEffect,
			Dragon->GetMesh(),
			FName("MouthSocket"),
			FVector::ZeroVector,
			FRotator(0.f, -90.f, 0.f),
			EAttachLocation::SnapToTargetIncludingScale,
			true
			);

		if (BreathEffectComponent)
		{
			BreathEffectComponent->SetRelativeScale3D(FVector(2.f));
		}
	}
}

void ADragonBreathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SprayDamage();
}

void ADragonBreathActor::SprayDamage()
{
	if (!Dragon) return;
	
	FVector Forward = FVector::ZeroVector;

	if (Dragon)
	{
		if (USkeletalMeshComponent* Mesh = Dragon->GetMesh())
		{
			const FTransform SocketTransform = Mesh->GetSocketTransform("MouthSocket", RTS_World);
			FRotator AdjustedRot = SocketTransform.GetRotation().Rotator();
			AdjustedRot.Pitch += -90.f;
			Forward = AdjustedRot.Vector();
		}
	}

	if (Forward.IsNearlyZero())
	{
		Forward = GetActorForwardVector();
	}
	
	const FVector Start = GetActorLocation();
	const FVector End = Start + Forward * BoxLength;
	const FVector BoxHalfExtent(BoxLength * 0.5f, BoxWidth * 0.5f, BoxHeight * 0.5f);
	const FQuat BoxRotation = FRotationMatrix::MakeFromX(Forward).ToQuat();

#if !(UE_BUILD_SHIPPING)
		DrawDebugBox(
			GetWorld(),
			(Start + End) * 0.5f,
			BoxHalfExtent,
			BoxRotation,
			FColor::Orange,
			false,
			0.05f
			);
#endif

	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (Dragon)
	{
		Params.AddIgnoredActor(Dragon);
	}
	
	bool bHit = GetWorld()->SweepMultiByChannel(
							Hits,
								Start,
								End,
								BoxRotation,
								ECC_Pawn,
								FCollisionShape::MakeBox(BoxHalfExtent),
								Params);

	if (bHit)
	{
		for (const FHitResult& Hit : Hits)
		{
			ACharacter* HitCharacter = Cast<ACharacter>(Hit.GetActor());
			if (!HitCharacter || HitCharacter == Dragon) continue;
			if (HitCharacter->GetClass() == Dragon->GetClass()) continue;

			//HitDatas[CurrAttackIndex].SendDamage(Dragon, this, HitCharacter);
		}
	}
}



