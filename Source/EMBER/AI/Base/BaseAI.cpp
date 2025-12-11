#include "BaseAI.h"

#include "AnimInstance/BaseAIAnimInstance.h"
#include "CAIController.h"
#include "C_CharacterMovementComponent.h"
#include "C_StateComponent.h"
#include "MontageSystemComponent.h"
#include "StatusComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/BehaviorTree/CBehaviorTreeComponent.h"
#include "AIComponent/CAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Managers/EquipmentManagerComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Item/Drop/LootTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"

ABaseAI::ABaseAI()
{
	PrimaryActorTick.bCanEverTick = true;

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("Status Component"));
	MontageComponent = CreateDefaultSubobject<UMontageSystemComponent>(TEXT("Montage Component"));
	EquipComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("Equip Component"));
	AIState = CreateDefaultSubobject<UC_StateComponent>(TEXT("AI State"));
	BehaviorTreeComponent = CreateDefaultSubobject<UCBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	AIMoveComponent = CreateDefaultSubobject<UC_CharacterMovementComponent>(TEXT("Move Component"));
	WeaponComponent = CreateDefaultSubobject<UCAIWeaponComponent>(TEXT("Weapon Component"));
	AIControllerClass = ACAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// WalkSpeed = 200.0f;
	// RunSpeed = 700.0f;

	WidgetTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("WidgetTrigger"));
	WidgetTrigger->SetupAttachment(RootComponent);
	WidgetTrigger->SetSphereRadius(10000.f);
	WidgetTrigger->SetCollisionProfileName(TEXT("Trigger"));
	WidgetTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WidgetTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseAI::BeginPlay()
{
	Super::BeginPlay();
	if (WalkSpeed != 0)
		AIMoveComponent->SetWalkSpeed(WalkSpeed);
	if (RunSpeed != 0)
		AIMoveComponent->SetRunSpeed(RunSpeed);
	if (SprintSpeed != 0)
		AIMoveComponent->SetSprintSpeed(SprintSpeed);
	AIState.Get()->SetIdleMode();
	//if (ACAIController* AIController = Cast<ACAIController>(GetController()))
	//{
	//	BlackboardComp = AIController->GetBlackboardComponent();
	//}
	//ACAIController* Controller = Cast<ACAIController>(GetController());
	// Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAI::OnTargetPerceptionUpdated);
	// SetWalkSpeed();

	if (WidgetTrigger)
	{
		WidgetTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABaseAI::OnTriggerBeginOverlap);
		WidgetTrigger->OnComponentEndOverlap.AddDynamic(this, &ABaseAI::OnTriggerEndOverlap);
	}
}

float ABaseAI::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                          AActor* DamageCauser)
{
	if (StatusComponent->GetHp() <= 0.0f)
		return 0;
	if (!HasAuthority()) 
		return 0;
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if(ActualDamage <= 0.0f)
	{
		UE_LOG(LogTemp, Error, L"Damage is 0");
		return ActualDamage;
	}
//TODOS 작업
	DamageData.Causer = DamageCauser;
	DamageData.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Power = ActualDamage;
	//FActionDamageEvent* event = (FActionDamageEvent*)&DamageEvent;
	//if (event->DamageData->Montages != nullptr)
	//	DamageData.Montage = event->DamageData->Montages;
	//if (event->DamageData->PlayRate)
	//	DamageData.PlayRate = event->DamageData->PlayRate;
	AIState->SetHittdMode();
	MulticastHitted(ActualDamage, DamageEvent, EventInstigator, DamageCauser);

	//if (AAIController* AIController = Cast<AAIController>(GetController()))
	//{
	//	if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
	//	{
	//		// BlackboardComponent->SetValueAsBool("IsHit", true);
	//		BehaviorTreeComponent->SetBlackboard_Object("TargetActor", EventInstigator->GetOwner());
	//		if (!BlackboardComponent->GetValueAsBool("IsOriginLocationSet"))
	//		{
	//			BehaviorTreeComponent->SetBlackboard_Vector("OriginLocation",GetActorLocation());
	//			BehaviorTreeComponent->SetBlackboard_Bool("IsOriginLocationSet", true);
	//		}
	//	}
	//}

   LastDamageCauser = DamageCauser;

	return ActualDamage;
}

void ABaseAI::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleBeginOverlap(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("WidgetTrigger Overlap"));
}

void ABaseAI::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HandleEndOverlap(OtherActor);
}

UC_CharacterMovementComponent* ABaseAI::GetAIMovement() const
{
	return AIMoveComponent;
}

void ABaseAI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseAI, DamageData);
}

void ABaseAI::MulticastHitted_Implementation(float Damage, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	StatusComponent->Damage(DamageData.Power);
	if (StatusComponent->GetHp() <= 0.0f)
	{
		OnDeath();
		return;
	}
	MontageComponent.Get()->PlayMontage(EStateType::Hitted);
	if (HasAuthority() == true)
	{
		UE_LOG(LogTemp, Error, L"server hp %f", StatusComponent->GetHp());
	}
	else
	{
		UE_LOG(LogTemp, Error, L"hp %f", StatusComponent->GetHp());
	}

	// if (DamageData.Character != nullptr)
	// 	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageData.Character->GetActorLocation()));

}

void ABaseAI::OnRep_Hitted()
{
	if (DamageData.Character != nullptr)
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageData.Character->GetActorLocation()));
}

void ABaseAI::PlaySound(AISoundCategory InSoundType)
{
	if (SoundAttenuation == nullptr)
	{
		UE_LOG(LogTemp, Error, L"[%s :: %s] SoundAttenuation is null", *GetClass()->GetName(), TEXT(__FUNCTION__));
		return;
	}
	if (AISounds[(int32)InSoundType] == nullptr)
	{
		UE_LOG(LogTemp, Error, L"[%s :: %s] %s is null", *GetClass()->GetName(), TEXT(__FUNCTION__), *UEnum::GetValueAsString(InSoundType));
		return;
	}
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AISounds[(int32)InSoundType], GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, SoundAttenuation);
}

void ABaseAI::OnDeath()
{
	UE_LOG(LogTemp, Display, TEXT("OnDeath"));
	this->GetCharacterMovement()->GravityScale = 1.0f;
	AIState->SetDeadMode();
	bIsDie = true;

	//퍼셉션 제거
	// Perception->SetSenseEnabled(UAISense_Sight::StaticClass(), false);

	if (HasAuthority())
	{
		FMonsterDiedMessage DeathMessage;
		DeathMessage.MonsterID = this->MonsterID;
		DeathMessage.DeathLocation = this->GetActorLocation();
		DeathMessage.KillerActor = this->LastDamageCauser;

		if (UWorld* World = GetWorld())
		{
			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(World);
			FGameplayTag MessageChannel = FGameplayTag::RequestGameplayTag(FName("Event.Monster.Died")); 
			MessageSubsystem.BroadcastMessage(MessageChannel, DeathMessage);
			UE_LOG(LogTemp, Log, TEXT("[SERVER] ABaseAI: Broadcasted FMonsterDiedMessage for %s"), *MonsterID.ToString());
		}
	}

	//이동, 애니메이션 제거
	if (GetController())
	{
		GetController()->StopMovement();
	}

	MontageComponent->PlayMontage(EStateType::Dead);
	//if (UBaseAIAnimInstance* AnimInstance = Cast<UBaseAIAnimInstance>(GetMesh()->GetAnimInstance()))
	//{
	//	// SetWalkSpeed();
	//	AnimInstance->StopAllMontages(0.0f);
	//}
	DetachFromControllerPendingDestroy();
}

void ABaseAI::EndDeath()
{
	WeaponComponent->DestroyWeapon();
	Destroy();
}

//void ABaseAI::OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
//{
//	//TODOS 원상복귀 해야될 수 있음
//	ACAIController* BaseAIController = Cast<ACAIController>(Cast<AAIController>(GetController()));
//	if (BaseAIController == nullptr)
//		BaseAIController = Cast<ACAIController>(GetController());
//	//ABaseAIController* BaseAIController = Cast<ABaseAIController>(Cast<AAIController>(GetController()));
//	BlackboardComp = BaseAIController->GetBlackboardComponent();
//}

UBehaviorTree* ABaseAI::GetBehaviorTree() const
{
	if (BehaviorTree == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Behavior Tree is null");
		return nullptr;
	}
	return BehaviorTree;
}

void ABaseAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bDebug == true)
	{
		const FSoundAttenuationSettings& sound = SoundAttenuation->Attenuation;
		DrawDebugSphere(GetWorld(),GetActorLocation(), sound.FalloffDistance,30,FColor::Blue);
		DrawDebugSphere(GetWorld(),GetActorLocation(), sound.AttenuationShapeExtents.X,30,FColor::Green);
	}
}
