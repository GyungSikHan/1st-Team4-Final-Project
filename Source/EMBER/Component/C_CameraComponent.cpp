#include "Component/C_CameraComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UC_CameraComponent::UC_CameraComponent()
{
}


void UC_CameraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	bFixedCamera = false;
	bControlRotation = false;
}

void UC_CameraComponent::EnableControlRotation()
{
	bControlRotation = true;
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}
void UC_CameraComponent::DisableControlRotation()
{
	bControlRotation = false;
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UC_CameraComponent::OnLook(const FInputActionValue& Value)
{
	bFixedCamera = false;
	if(bFixedCamera == true)
		return;

	FVector2D controlVector = Value.Get<FVector2D>();

	OwnerCharacter->AddControllerYawInput(controlVector.X * GetWorld()->GetDeltaSeconds() * HorizontalLook);
	OwnerCharacter->AddControllerPitchInput(controlVector.Y * GetWorld()->GetDeltaSeconds() * VerticalLook);
}

void UC_CameraComponent::PostInitProperties()
{
	Super::PostInitProperties();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Character is null"));
		return;
	}
}
