#include "C_CharacterMovementComponent.h"

#include "CAIController.h"
#include "C_StateComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UC_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = GetCharacterOwner();
	UC_StateComponent* StateCom = Cast<UC_StateComponent>(GetOwner()->FindComponentByClass<UC_StateComponent>());
	State = StateCom;
}

void UC_CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UC_CharacterMovementComponent::GetCanMove() const
{
	return bCanMove;
}

bool UC_CharacterMovementComponent::GetCanDash() const
{
	return bIsDash;
}

float UC_CharacterMovementComponent::GetCurrentSpeed() const
{
	return Speed[(int32)CurrentSpeedType];
}

void UC_CharacterMovementComponent::EnableMove()
{
	bCanMove = true;
}

void UC_CharacterMovementComponent::DisableMove()
{
	bCanMove = false;
}

//float UC_CharacterMovementComponent::GetMaxSpeed() const
//{
//	float MaxMoveSpeed = Super::GetMaxSpeed();
//
//	switch (MovementMode)
//	{
//	case MOVE_Walking:
//	case MOVE_NavWalking:
//		{
//			float DirectionCheck;
//			float DirectionDot = GetOwner()->GetActorForwardVector().Dot(Velocity.GetSafeNormal());
//			if (DirectionDot < 0.25f)
//			{
//				if (DirectionDot > -0.25f)
//				{
//					DirectionCheck = MaxMoveSpeed * LeftRightMovePercent;
//				}
//				else
//				{
//					DirectionCheck = MaxMoveSpeed * BackwardMovePercent;
//				}
//			}
//			else
//			{
//				DirectionCheck = MaxMoveSpeed;
//			}
//			MaxMoveSpeed = DirectionCheck;
//		}
//	case MOVE_Falling:
//		return MaxMoveSpeed;
//	case MOVE_Swimming:
//		return MaxSwimSpeed;
//	case MOVE_Flying:
//		return MaxFlySpeed;
//	case MOVE_Custom:
//		return MaxCustomMovementSpeed;
//	case MOVE_None:
//	default:
//		return 0.f;
//	}
//
//	return MaxMoveSpeed;
//}

void UC_CharacterMovementComponent::OnMove(const FInputActionValue& Value)
{
	bCanMove = true;
	if (bCanMove == false)
		return;
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (MoveVector.IsNearlyZero())
	{
		return;
	}

	if (OwnerCharacter && OwnerCharacter->GetController())
	{
		const FRotator Rotation = OwnerCharacter->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		// 이동 입력 적용
		AddInputVector(ForwardDirection * MoveVector.Y);
		AddInputVector(RightDirection * MoveVector.X);
		//OnMoveDelegate.Broadcast(MoveVector);
	}
}

void UC_CharacterMovementComponent::OnJump()
{
	OwnerCharacter->Jump();
}

void UC_CharacterMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UC_CharacterMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UC_CharacterMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UC_CharacterMovementComponent::SetSpeed(ESpeedType SpeedType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)SpeedType];
}

void UC_CharacterMovementComponent::SetFlySpeed(EFlySpeedType FlySpeedType)
{
	OwnerCharacter->GetCharacterMovement()->MaxFlySpeed = FlySpeed[(int32)FlySpeedType];
	ACAIController* Controller = Cast<ACAIController>(OwnerCharacter->GetController());
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Controller->GetBlackboardComponent());
	BlackboardComponent->SetValueAsFloat("FlySpeed", FlySpeed[(int32)FlySpeedType]);
}

void UC_CharacterMovementComponent::SetWalkSpeed(int32 InSpeed)
{
	InSpeed = UKismetMathLibrary::Clamp(InSpeed, 0, 600);
	Speed[(int32)ESpeedType::Walk] = InSpeed;
}

void UC_CharacterMovementComponent::SetRunSpeed(int32 InSpeed)
{
	InSpeed = UKismetMathLibrary::Clamp(InSpeed, 0, 600);
	Speed[(int32)ESpeedType::Run] = InSpeed;
}

void UC_CharacterMovementComponent::SetSprintSpeed(int32 InSpeed)
{
	InSpeed = UKismetMathLibrary::Clamp(InSpeed, 0, 600);
	Speed[(int32)ESpeedType::Sprint] = InSpeed;
}
