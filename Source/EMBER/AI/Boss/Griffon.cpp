#include "AI/Boss/Griffon.h"
#include "CAIController.h"
#include "C_StateComponent.h"
#include "StatusComponent.h"
#include "EmberPlayerCharacter.h"
#include "CUserWidget_AIHP.h"
#include "UI/EmberHUD.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/GameplayStatics.h"
#include "CommonActivatableWidget.h"

AGriffon::AGriffon()
{
	// PrimaryActorTick.bCanEverTick = true;
}

void AGriffon::BeginPlay()
{
	ABaseAI::BeginPlay();
}

// void AGriffon::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	if (GetActorLocation().Z > 700.0f)
// 	{
// 		GetCharacterMovement()->Velocity.Z = 0.0f;
// 	}
//
// 	if (GetCharacterMovement()->IsMovingOnGround())
// 	{
// 		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
// 	}
// }

float AGriffon::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                           class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ACAIController* AIController = Cast<ACAIController>(GetController()))
	{
		UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(AIController->GetBlackboardComponent());
		if (!BlackboardComponent->GetValueAsBool("IsHalfHP"))
		{
			if (StatusComponent->GetHp() < (StatusComponent->GetMaxHp() / 2))
			{
				if (!BlackboardComponent->GetValueAsBool("IsHalfHP"))
				BlackboardComponent->SetValueAsBool("IsHalfHP", true);
				BlackboardComponent->SetValueAsInt("RandomPattern", 3);
			}
		}
	}
	
	return ActualDamage;
}

void AGriffon::HandleBeginOverlap(AActor* OtherActor)
{
	if (AEmberPlayerCharacter* Player = Cast<AEmberPlayerCharacter>(OtherActor))
	{
		if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
		{
			if (AEmberHUD* EmberHUD = Cast<AEmberHUD>(PC->GetHUD()))
			{
				if (EmberHUD->MainScreenWidget)
				{
					if (UUserWidget* RootUserWidget = Cast<UUserWidget>(EmberHUD->MainScreenWidget))
					{
						if (UWidget* Found = RootUserWidget->GetWidgetFromName(TEXT("WBP_HUD_AIHP")))
						{
							if (UCUserWidget_AIHP* AIHPWidget = Cast<UCUserWidget_AIHP>(Found))
							{
								AIHPWidget->SetTargetAI(this);
								UE_LOG(LogTemp, Warning, TEXT("SetTargetAI(this) called from HandleBeginOverlap"));
							}
						}
					}
				}
			}
		}
	}
}

void AGriffon::HandleEndOverlap(AActor* OtherActor)
{
	if (AEmberPlayerCharacter* Player = Cast<AEmberPlayerCharacter>(OtherActor))
	{
		if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
		{
			if (AEmberHUD* EmberHUD = Cast<AEmberHUD>(PC->GetHUD()))
			{
				if (EmberHUD->MainScreenWidget)
				{
					if (UUserWidget* RootUserWidget = Cast<UUserWidget>(EmberHUD->MainScreenWidget))
					{
						if (UWidget* Found = RootUserWidget->GetWidgetFromName(TEXT("WBP_HUD_AIHP")))
						{
							if (UCUserWidget_AIHP* AIHPWidget = Cast<UCUserWidget_AIHP>(Found))
							{
								if (AIHPWidget->GetTargetAI() == this)
								{
									AIHPWidget->SetTargetAI(nullptr);
									UE_LOG(LogTemp, Warning, TEXT("SetTargetAI(nullptr) called from HandleEndOverlap"));
								}
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("AIHPWidget cast failed in HandleEndOverlap"));
							}
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("AIHPWidget not found in HandleEndOverlap"));
						}
					}
				}
			}
		}
	}
}

//void AGriffon::OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
//{
//	Super::OnTargetPerceptionUpdated(UpdatedActor, Stimulus);
//	
//	// SetBlackboardVector("OriginLocation", GetActorLocation());
//}
