#include "AI/AnimInstance/DragonAnimInstance.h"

#include "NiagaraFunctionLibrary.h"
#include "Boss/Dragon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraSystem.h"

void UDragonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Dragon = Cast<ADragon>(TryGetPawnOwner());
}

void UDragonAnimInstance::AnimNotify_SpawnSpit()
{
	if (Dragon)
	{
		Dragon->SpawnSpit();
	}
}

void UDragonAnimInstance::AnimNotify_SpawnEffect()
{
	if (Dragon && SpawnNiagara)
	{
		FVector SocketLocation = Dragon->GetMesh()->GetSocketLocation(FName("SpawnSocket"));
		FRotator SocketRotation = Dragon->GetMesh()->GetSocketRotation(FName("SpawnSocket"));

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			Dragon->GetWorld(),
			SpawnNiagara,
			SocketLocation,
			SocketRotation,
			FVector(2.f)
			);
	}
}

void UDragonAnimInstance::AnimNotify_SpawnBreath()
{
	if (Dragon)
	{
		Dragon->SpawnBreath();
	}
}

void UDragonAnimInstance::AnimNotify_BreathEnd()
{
	if (Dragon)
	{
		Dragon->StopBreath();
	}
}

void UDragonAnimInstance::AnimNotify_Fly()
{
	if (Dragon)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fly Notify"));
		Dragon->LaunchCharacter(FVector(0.f, 0.f, Dragon->GetLaunchZPower()), false, true);
	}
}

void UDragonAnimInstance::AnimNotify_Land()
{
	if (Dragon)
	{
		Dragon->GetCharacterMovement()->GravityScale = 1.f;
	}
}

void UDragonAnimInstance::AnimNotify_Glide()
{
	if (Dragon)
	{
		//if (!Dragon->)
	}
}



