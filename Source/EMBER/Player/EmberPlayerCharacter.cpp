// Fill out your copyright notice in the Description page of Project Settings.
#include "EmberPlayerCharacter.h"
#include "EngineUtils.h"
#include "AbilitySystemComponent.h"
#include "ArmorComponent.h"
#include "C_CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "C_CharacterMovementComponent.h"
#include "EmberPlayerState.h"
#include "GameData.h"
#include "StatusComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Managers/EquipmentManagerComponent.h"
#include "..\GameInfo/GameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "Input/EmberEnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/GameModeBase.h"
#include "Interaction/RespawnSubsystem.h"

AEmberPlayerCharacter::AEmberPlayerCharacter(const FObjectInitializer& Init)
	: Super(Init.SetDefaultSubobjectClass<UC_CharacterMovementComponent>
		(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraLogicComp = CreateDefaultSubobject<UC_CameraComponent>(TEXT("CameraLogic"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManager"));
	CharacterInputComponent = CreateDefaultSubobject<UCharacterInputComponent>(TEXT("CharacterInput"));

	MontageComponent = CreateDefaultSubobject<UMontageSystemComponent>(TEXT("MontageComponent"));
	ArmorComponent = CreateDefaultSubobject<UArmorComponent>(TEXT("ArmorComponent"));

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	StatusComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	StatusComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	Tags.Add("Player");
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AEmberPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, L"start max hp %f hp %f ", StatusComponent->GetMaxHp(), StatusComponent->GetHp());
	MovementComponent->OnRun();
	if (StatusComponent->GetMaxHp() <= 0.0f)
		StatusComponent->SetMaxHp(100);
	if (StatusComponent->GetMaxStamina() <= 0.0f)
		StatusComponent->SetMaxStamina(100);
	CameraLogicComp->DisableControlRotation();
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController != nullptr)
	{
		playerController->PlayerCameraManager->ViewPitchMin = PitchRange.X;
		playerController->PlayerCameraManager->ViewPitchMax = PitchRange.Y;
	}

	//TODOS
	if (ArmorComponent != nullptr)
	{
		if (HasAuthority() == true)
		{
			ArmorComponent->InitializeArmorForLateJoiners();
		}
	}
	
	if (HasAuthority())
	{
		StartPassiveTemperatureDrop();
	}
}

void AEmberPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MovementComponent = Cast<UC_CharacterMovementComponent>(GetCharacterMovement());
}

void AEmberPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AEmberPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (ArmorComponent != nullptr)
		ArmorComponent->InitializeArmorForLateJoiners();

	if (AEmberPlayerState* EmberPlayerState = GetPlayerState<AEmberPlayerState>())
	{
		SetAbilitySystemComponent(EmberPlayerState->GetAbilitySystemComponent());
	}
}

void AEmberPlayerCharacter::InitAbilityActorInfo()
{
	AEmberPlayerState* EmberPlayerState = GetPlayerState<AEmberPlayerState>();
	check(EmberPlayerState);
	EmberPlayerState->InitAbilitySystemComponent();
	AbilitySystemComponent = EmberPlayerState->GetAbilitySystemComponent();
}

void AEmberPlayerCharacter::PostNetInit()
{
	Super::PostNetInit();
	UE_LOG(LogTemp, Warning, L"PostNetInit Call");
	if (ArmorComponent != nullptr)
		ArmorComponent->InitializeArmorForLateJoiners();
}

void AEmberPlayerCharacter::SetControlRotation(bool bEnable)
{
	if (bEnable)
	{
		CameraLogicComp->EnableControlRotation();
	}
	else
	{
		CameraLogicComp->DisableControlRotation();
	}
}

void AEmberPlayerCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void AEmberPlayerCharacter::OnDeathFinished(AActor* OwningActor)
{
	// 향후 구현 예정
}

void AEmberPlayerCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	}
	
	if (UCharacterMovementComponent* LyraCharacterMovement = Cast<UCharacterMovementComponent>(GetCharacterMovement()))
	{
		LyraCharacterMovement->Velocity = FVector(0.f, 0.f, LyraCharacterMovement->Velocity.Z);
		LyraCharacterMovement->UpdateComponentVelocity();
	}

	bUseControllerRotationYaw = false;
}


void AEmberPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}
void AEmberPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called every frame
void AEmberPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEmberPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!InputConfigDataAsset)
	{
		return;
	}
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	CharacterInputComponent->InitializePlayerInput(PlayerInputComponent);

	if (!IsLocallyControlled())
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	Subsystem->AddMappingContext(InputConfigDataAsset->InputMappingContext, 0);

	UEmberEnhancedInputComponent* WarriorInputComponent = CastChecked<UEmberEnhancedInputComponent>(PlayerInputComponent);

	WarriorInputComponent->BindNativeAction(InputConfigDataAsset, EmberGameplayTags::InputTag_Movement_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	WarriorInputComponent->BindNativeAction(InputConfigDataAsset, EmberGameplayTags::InputTag_Movement_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	WarriorInputComponent->BindNativeAction(InputConfigDataAsset, EmberGameplayTags::InputTag_Movement_Sprint, ETriggerEvent::Triggered, this, &ThisClass::StartSprint);
	WarriorInputComponent->BindNativeAction(InputConfigDataAsset, EmberGameplayTags::InputTag_Movement_Sprint, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
}

UAbilitySystemComponent* AEmberPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEmberPlayerCharacter::Move(const FInputActionValue& value)
{
	if (MovementComponent)
	{
		MovementComponent->OnMove(value);
	}
}

void AEmberPlayerCharacter::Look(const FInputActionValue& value)
{
	CameraLogicComp->OnLook(value);
}

void AEmberPlayerCharacter::StartSprint(const FInputActionValue& value)
{
	if (StatusComponent->GetStamina() < UseSprintAmount)
	{
		bIsSprint = true;
		MovementComponent->OnRun();
		return;
	}
	if (GetCharacterMovement())
	{
		MovementComponent->OnSprint();
		StatusComponent->UseStamina(UseSprintAmount);
		GetWorldTimerManager().ClearTimer(StaminaRegenHandle);
	}
}

void AEmberPlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		bIsSprint = false;
		MovementComponent->OnRun();
		GetWorldTimerManager().SetTimer(
			StaminaRegenHandle,
			[this]()
			{
				StatusComponent->UseStamina(-0.2f); 
				if (StatusComponent->GetStamina() >= StatusComponent->GetMaxStamina())
				{
					GetWorldTimerManager().ClearTimer(StaminaRegenHandle);
				}
			},
			StaminaRegenInterval,
			true
		);
	}
}

void AEmberPlayerCharacter::Attack(const FInputActionValue& value)
{
	// EquipmentComponent에서 현재 무기 타입 가져오기
	FAttackData Data = EquipmentManagerComponent->GetAttackInfo();

	if (!Data.Montages)
	{
		MontageComponent->PlayMontage(Data.Montages);
		if (attackint == 2)
		{
			attackint = 0;
			return;
		}
		attackint++;

	}
}

float AEmberPlayerCharacter::GetMaxHP() const
{
	if (StatusComponent)
	{
		return StatusComponent->GetMaxHp();
	}
	return 0.f;
}

float AEmberPlayerCharacter::GetMaxStamina() const
{
	if (StatusComponent)
	{
		return StatusComponent->GetMaxStamina();
	}
	return 0.f;
}

float AEmberPlayerCharacter::GetMaxTemparature() const
{
	if (StatusComponent)
	{
		return StatusComponent->GetMaxTemperature();
	}
	return 0.f;
}

float AEmberPlayerCharacter::GetCurrentHP() const
{
	if (StatusComponent)
	{
		return StatusComponent->GetHp();
	}
	return 0.f;
}

float AEmberPlayerCharacter::GetCurrentStamina() const
{
	if (StatusComponent)
	{
		return StatusComponent->GetStamina();
	}
	return 0.f;
}

float AEmberPlayerCharacter::GetCurrentTemparature() const
{
	if (StatusComponent)
	{
		return StatusComponent->GetTemperature();
	}
	return 0.f;
}

void AEmberPlayerCharacter::OnLeftClick(const FInputActionValue& Value)
{
	static const FGameplayTag LeftClickTag = EmberGameplayTags::InputTag_Attack_MainHand;
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(LeftClickTag));

}

float AEmberPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (HasAuthority() == false)
		return 0.0f;

	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (damage <= 0.0f)
	{
		UE_LOG(LogTemp, Error, L"Damage is 0");
		return damage;
	}

	//StatusComponent->OnRep_Damage(damage);

	DamageData.Causer = DamageCauser;
	DamageData.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Power = damage;
	//FActionDamageEvent* event = (FActionDamageEvent*)&DamageEvent;
	//DamageData.Montage = event->DamageData->Montages;
	//DamageData.PlayRate = event->DamageData->PlayRate;
	MulticastHitted(damage, DamageEvent, EventInstigator, DamageCauser);

	/*if (UAbilitySystemComponent* EmberASC = GetAbilitySystemComponent())
	{
		FGameplayEventData Payload;
		Payload.EventTag = EmberGameplayTags::GameplayEvent_HitReact;
		Payload.Target = this;

		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}*/
	
	return damage;
}

void AEmberPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEmberPlayerCharacter, DamageData);
}

void AEmberPlayerCharacter::MulticastHitted_Implementation(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	StatusComponent->Damage(DamageData.Power);
	if (UAbilitySystemComponent* EmberASC = GetAbilitySystemComponent())
	
		if (StatusComponent->GetHp() <= 0.0f)
		{
			//TODOS PlayerDead 상태변화
			FGameplayEventData Payload;
			Payload.EventTag = EmberGameplayTags::GameplayEvent_Death;
			Payload.Instigator = DamageCauser;
			Payload.Target = this;
			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
			
			OnDeath();
			return;
		}
		else
		{
			FGameplayEventData Payload;
			Payload.EventTag = EmberGameplayTags::GameplayEvent_HitReact;
			Payload.Instigator = DamageCauser;
			Payload.Target = this;
			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}

	//MontageComponent->PlayMontage(EStateType::Hitted);
	// 애니메이션 종료시 캐릭터 상태 관리를 위해 GaemplayAbility에서 애니메이션 재생 구현
	/*
	if (HasAuthority() == true)
	{
		UE_LOG(LogTemp, Error, L"server hp %f", StatusComponent->GetHp());
	}
	else
	{
		UE_LOG(LogTemp, Error, L"hp %f", StatusComponent->GetHp());
	}
	*/

	if (DamageData.Character != nullptr)
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageData.Character->GetActorLocation()));
}

void AEmberPlayerCharacter::OnRep_Hitted()
{
	if (DamageData.Character != nullptr)
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageData.Character->GetActorLocation()));
}

void AEmberPlayerCharacter::OnDeath()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//MontageComponent->PlayMontage(EStateType::Dead);
	if (HasAuthority())
	{
		AController* PC = GetController();
		if (PC)
		{
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, FTimerDelegate::CreateLambda([this, PC]()
			{
				URespawnSubsystem* RespawnSubsystems = GetGameInstance()->GetSubsystem<URespawnSubsystem>();
				FTransform Respawn = RespawnSubsystems->GetRespawnTransform();
				if (UWorld* World = GetWorld())
				{
					if (AGameModeBase* GM = World->GetAuthGameMode<AGameModeBase>())
					{
						//스폰 위치 설정은 GameMode 쪽에서 처리gh
						GM->RestartPlayerAtTransform(PC,Respawn);
					}
				}
			}), 5.0f, false);
		}
	}
}
void AEmberPlayerCharacter::EndDeath()
{
	Destroy();
}

void AEmberPlayerCharacter::ApplyWarmingEffect_Implementation()
{
	if (!HasAuthority()) return;
	
	StopPassiveTemperatureDrop();

	GetWorldTimerManager().SetTimer(WarmingTimerHandle, this, &AEmberPlayerCharacter::OnWarmingTick, 1.0f, true, 0.0f);
    
	UE_LOG(LogTemp, Warning, TEXT("Player entered warming zone. Starting warming timer."));
}

void AEmberPlayerCharacter::RemoveWarmingEffect_Implementation()
{
	if (!HasAuthority()) return;

	GetWorldTimerManager().ClearTimer(WarmingTimerHandle);
	
    StartPassiveTemperatureDrop();

	UE_LOG(LogTemp, Warning, TEXT("Player left warming zone. Clearing warming timer."));
}

void AEmberPlayerCharacter::OnWarmingTick()
{
	if (StatusComponent)
	{
		StatusComponent->AddTemperature(WarmingAmountPerSecond);
		UE_LOG(LogTemp, Log, TEXT("Warming tick: Current Temperature is %f"), StatusComponent->GetTemperature());
	}
}

void AEmberPlayerCharacter::StartPassiveTemperatureDrop()
{
	if (!HasAuthority() || GetWorldTimerManager().IsTimerActive(TemperatureDropTimerHandle)) return;

	GetWorldTimerManager().SetTimer(TemperatureDropTimerHandle, this, &AEmberPlayerCharacter::OnTemperatureDropTick, TemperatureDropInterval, true);
	UE_LOG(LogTemp, Warning, TEXT("Starting passive temperature drop."));
}

void AEmberPlayerCharacter::StopPassiveTemperatureDrop()
{
	if (!HasAuthority()) return;

	GetWorldTimerManager().ClearTimer(TemperatureDropTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("Stopping passive temperature drop."));
}

void AEmberPlayerCharacter::OnTemperatureDropTick()
{
	if (StatusComponent)
	{
		StatusComponent->UseTemperature(TemperatureDropAmount);
		if (StatusComponent->GetTemperature()/StatusComponent->GetMaxTemperature() <= 0.4)
		{
			MovementComponent->OnWalk();
		}
		else
		{
			if (bIsSprint == false)
				MovementComponent->OnRun();
			else
				MovementComponent->OnSprint();
		}
		UE_LOG(LogTemp, Log, TEXT("Passive drop tick: Current Temperature is %f"), StatusComponent->GetTemperature());
	}
}
// 금지 구역(Exclusion Zone) 체크 함수
bool AEmberPlayerCharacter::IsInExclusionZone(const FVector& SpawnLocation)
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->ActorHasTag("AIExclusion"))
		{
			// 볼륨(AVolume)일 경우 EncompassesPoint 사용
			if (Actor->IsA(AVolume::StaticClass()))
			{
				AVolume* Volume = Cast<AVolume>(Actor);
				if (Volume && Volume->EncompassesPoint(SpawnLocation))
				{
					return true;
				}
			}
			else
			{
				// 일반 액터면 컴포넌트 바운딩 박스 사용
				FBox Bounds = Actor->GetComponentsBoundingBox();
				if (Bounds.IsInside(SpawnLocation))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool AEmberPlayerCharacter::IsInBeginnerZone(const FVector& Location)
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->ActorHasTag("BeginnerZone"))
		{
			// 볼륨이면 EncompassesPoint, 일반 액터면 Bounds
			if (Actor->IsA(AVolume::StaticClass()))
			{
				AVolume* Volume = Cast<AVolume>(Actor);
				if (Volume && Volume->EncompassesPoint(Location))
					return true;
			}
			else
			{
				FBox Bounds = Actor->GetComponentsBoundingBox();
				if (Bounds.IsInside(Location))
					return true;
			}
		}
	}
	return false;
}

// AI 스폰 헬퍼 함수
void AEmberPlayerCharacter::SpawnAI(const TArray<TSubclassOf<APawn>>& AIClasses, const FString& AIType, const FVector& PlayerLocation)
{
	if (AIClasses.Num() == 0)
	{
		return;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys || !NavSys->GetMainNavData())
	{
		return;
	}

	int32 Index = FMath::RandRange(0, AIClasses.Num() - 1);
	TSubclassOf<APawn> SelectedClass = AIClasses[Index];

	FVector SpawnLocation;
	bool bFoundValidLocation = false;

	for (int32 j = 0; j < MaxAttempts; ++j)
	{
		FNavLocation NavLoc;
		if (NavSys->GetRandomReachablePointInRadius(PlayerLocation, SpawnRadius, NavLoc))
		{
			SpawnLocation = NavLoc.Location;

			if (!IsInExclusionZone(SpawnLocation))
			{
				// 땅 위로 위치 보정 추가
				SpawnLocation = FindGroundLocation(GetWorld(), SpawnLocation, 3000.0f);
				bFoundValidLocation = true;
				break;
			}
		}
	}

	if (!bFoundValidLocation)
	{
		return;
	}

	// ▶▶▶ 지면에서 40유닛 위로 위치 조정 ◀◀◀
	const float ZOffset = 40.0f;
	FVector FinalSpawnLocation = SpawnLocation + FVector(0, 0, ZOffset);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* SpawnedEnemy = GetWorld()->SpawnActor<APawn>(
		SelectedClass,
		FinalSpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);
}

// 지면 위치 찾기 함수
FVector AEmberPlayerCharacter::FindGroundLocation(UWorld* World, const FVector& Start, float TraceDistance)
{
	FHitResult Hit;
	FVector TraceStart = Start + FVector(0, 0, 1000);

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(this);

	// 1. 스피어 트레이스
	const float SphereRadius = 100.0f;
	const float SafeTraceDistance = FMath::Max(TraceDistance, 3000.0f);
	FVector SafeTraceEnd = Start - FVector(0, 0, SafeTraceDistance);

	if (World->SweepSingleByChannel(
		Hit,
		TraceStart,
		SafeTraceEnd,
		FQuat::Identity,
		ECC_WorldStatic,
		FCollisionShape::MakeSphere(SphereRadius),
		Params))
	{
		FVector Candidate = Hit.ImpactPoint;
		FVector Adjusted = AdjustLocationForCollision(World, Candidate);
		return Adjusted;
	}

	// 2. 라인 트레이스
	FVector FallbackTraceEnd = Start - FVector(0, 0, 15000.0f);
	if (World->LineTraceSingleByChannel(Hit, TraceStart, FallbackTraceEnd, ECC_WorldStatic, Params))
	{
		FVector Candidate = Hit.ImpactPoint;
		FVector Adjusted = AdjustLocationForCollision(World, Candidate);
		DrawDebugPoint(World, Adjusted, 15, FColor::Yellow, false, 2.0f);
		return Adjusted;
	}

	// 3. 주변 지형 스캔
	FVector GroundLocation = Start;
	if (FindNearestGround(World, Start, GroundLocation, 500.0f))
	{
		FVector Adjusted = AdjustLocationForCollision(World, GroundLocation);
		DrawDebugPoint(World, Adjusted, 20, FColor::Blue, false, 2.0f);
		return Adjusted;
	}

	// 4. 최종 안전장치
	DrawDebugLine(World, TraceStart, FallbackTraceEnd, FColor::Red, false, 2.0f);
	return AdjustLocationForCollision(World, Start);
}

// 충돌 검사 및 위치 보정 함수
FVector AEmberPlayerCharacter::AdjustLocationForCollision(UWorld* World, const FVector& Candidate)
{
	if (!World) return Candidate;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (!CapsuleComp) return Candidate;

	const float CapsuleRadius = CapsuleComp->GetScaledCapsuleRadius();
	const float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	const FVector CapsuleOffset(0, 0, CapsuleHalfHeight);

	FCollisionQueryParams OverlapParams;
	OverlapParams.AddIgnoredActor(this);

	bool bOverlapped = World->OverlapAnyTestByChannel(
		Candidate + CapsuleOffset,
		FQuat::Identity,
		ECC_WorldStatic,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		OverlapParams
	);

	if (!bOverlapped) return Candidate;

	const float StepSize = 10.0f;
	const int32 MaxSteps = 20;

	for (int32 i = 1; i <= MaxSteps; ++i)
	{
		FVector TestLocation = Candidate + FVector(0, 0, i * StepSize);

		if (!World->OverlapAnyTestByChannel(
			TestLocation + CapsuleOffset,
			FQuat::Identity,
			ECC_WorldStatic,
			FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
			OverlapParams))
		{
			return TestLocation;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed to find collision-free position!"));
	return Candidate;
}

// 주변 지형 탐색 헬퍼 함수
bool AEmberPlayerCharacter::FindNearestGround(UWorld* World, const FVector& Origin, FVector& OutLocation, float Radius)
{
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(this);

	const int32 RayCount = 12;
	float BestDistance = BIG_NUMBER;
	bool bFound = false;

	for (int i = 0; i < RayCount; i++)
	{
		float Angle = 2 * PI * i / RayCount;
		FVector Direction(FMath::Cos(Angle), FMath::Sin(Angle), -1);
		FVector RayStart = Origin + FVector(0, 0, 500);
		FVector RayEnd = RayStart + Direction * Radius * 2;

		FHitResult LocalHit;
		if (World->LineTraceSingleByChannel(
			LocalHit,
			RayStart,
			RayEnd,
			ECC_WorldStatic,
			Params))
		{
			float Distance = FVector::Dist(Origin, LocalHit.ImpactPoint);
			if (Distance < BestDistance)
			{
				BestDistance = Distance;
				OutLocation = LocalHit.ImpactPoint;
				bFound = true;
			}
		}
	}
	return bFound;
}

void AEmberPlayerCharacter::SpawnRandomEnemy()
{
	if (IsInExclusionZone(GetActorLocation()))
	{
		return;
	}

	// 타입별 목표 수 설정
	const int32 TargetAggressiveCount = 7;
	const int32 TargetDefensiveCount = 3;

	FVector MyLocation = GetActorLocation();

	// 타입별 카운트
	int32 AggressiveCount = 0;
	int32 DefensiveCount = 0;

	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		APawn* Pawn = *It;
		if (!Pawn || Pawn == this) continue;

		for (auto& AIClass : AggressiveAIClasses)
		{
			if (Pawn->IsA(AIClass) && FVector::Dist(MyLocation, Pawn->GetActorLocation()) <= SpawnRadius)
			{
				AggressiveCount++;
				break;
			}
		}
		for (auto& AIClass : DefensiveAIClasses)
		{
			if (Pawn->IsA(AIClass) && FVector::Dist(MyLocation, Pawn->GetActorLocation()) <= SpawnRadius)
			{
				DefensiveCount++;
				break;
			}
		}
	}

	// 초보자 구역 안이면 선공 AI는 스폰하지 않음
	int32 MissingAggressive = 0;
	int32 MissingDefensive = 0;
	if (IsInBeginnerZone(MyLocation))
	{
		MissingDefensive = FMath::Max(0, TargetAggressiveCount + TargetDefensiveCount - DefensiveCount);
	}
	else
	{
		MissingAggressive = FMath::Max(0, TargetAggressiveCount - AggressiveCount);
		MissingDefensive = FMath::Max(0, TargetDefensiveCount - DefensiveCount);
	}

	// 비선공 AI 스폰
	for (int32 i = 0; i < MissingDefensive; ++i)
	{
		SpawnAI(DefensiveAIClasses, TEXT("비선공"), MyLocation);
	}
	// 일반 구역에서만 선공 AI 스폰
	if (!IsInBeginnerZone(MyLocation))
	{
		for (int32 i = 0; i < MissingAggressive; ++i)
		{
			SpawnAI(AggressiveAIClasses, TEXT("선공"), MyLocation);
		}
	}
}

void AEmberPlayerCharacter::DestroyFarAIs()
{
	FVector PlayerLocation = GetActorLocation();
	TArray<APawn*> AIsToDestroy;
	// 1. 삭제 대상 AI 수집
	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		APawn* Pawn = *It;
		if (!Pawn || Pawn == this) continue;

		// 관리 대상 AI인지 확인 (선공/비선공)
		bool bIsOurAI = false;
		for (auto& AIClass : AggressiveAIClasses)
		{
			if (Pawn->IsA(AIClass))
			{
				bIsOurAI = true;
				break;
			}
		}
		if (!bIsOurAI)
		{
			for (auto& AIClass : DefensiveAIClasses)
			{
				if (Pawn->IsA(AIClass))
				{
					bIsOurAI = true;
					break;
				}
			}
		}
		if (!bIsOurAI) continue;

		// 거리 체크
		float Distance = FVector::Dist(PlayerLocation, Pawn->GetActorLocation());
		if (Distance > SpawnRadius)
		{
			AIsToDestroy.Add(Pawn);
		}
	}

	// 2. 수집된 AI 삭제
	for (APawn* Pawn : AIsToDestroy)
	{
		Pawn->Destroy();
	}
}
