// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberTreeBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "GameInfo/GameplayTags.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interaction/Physics/WeaponImpactField.h"
#include "kismet/GameplayStatics.h"
#include "UI/Data/EmberItemData.h"

class AWeaponImpactField;

AEmberTreeBase::AEmberTreeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	
	RequiredTags.AddTag(EmberGameplayTags::Ability_Mining);

	AnchorField = CreateDefaultSubobject<UChildActorComponent>("AnchorField");
	AnchorField->SetHiddenInGame(true);
	AnchorField->SetupAttachment(GetRootComponent());

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollection");
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetNotifyBreaks(true);
	GeometryCollection->SetCollisionProfileName(TEXT("Mineable"));
	GeometryCollection->CanCharacterStepUpOn = ECB_No;
	GeometryCollection->SetupAttachment(GetRootComponent());

	RequiredTags.AddTag(EmberGameplayTags::Ability_Logging);
	
	EventTag = EmberGameplayTags::GameplayEvent_Reward_Item;
}

void AEmberTreeBase::BeginPlay()
{
	Super::BeginPlay();
	
	AFieldSystemActor* FieldSystemActor = Cast<AFieldSystemActor>(AnchorField->GetChildActor());
	GeometryCollection->InitializationFields.Add(FieldSystemActor);
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ThisClass::HandleBreakEvent);
	OnTakePointDamage.AddDynamic(this, &ThisClass::HandlePointDamage);
	
	InitResource();
}

void AEmberTreeBase::HandlePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                       FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                       const UDamageType* DamageType, AActor* InDamageCauser)
{
	if (HasAuthority() == false)
		return;

	if (ChaosImpactClass == nullptr)
		return;

	SetDynamic_Implementation(HitLocation);
	
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(HitLocation);
	AWeaponImpactField* WeaponImpact = GetWorld()->SpawnActorDeferred<AWeaponImpactField>(
		ChaosImpactClass,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		ESpawnActorScaleMethod::MultiplyWithRoot
	);
	
	if (WeaponImpact)
	{
		// TODO : 무기에 따른 충돌 물리 계산 필요
		float ForceMagnitude = 3000.f;
		float SphereScaleMultiplier = 0.58f;
		
		WeaponImpact->Init(ForceMagnitude, SphereScaleMultiplier);
		UGameplayStatics::FinishSpawningActor(WeaponImpact, SpawnTransform);
	}

	DamageCauser = InDamageCauser;
}

void AEmberTreeBase::HandleBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	if (HasAuthority() == false)
		return;
	
	// 메스 구하기
	CurrentBrokenMass += BreakEvent.Mass;

	/*GEngine->AddOnScreenDebugMessage(
		-1,                   // Key (-1 = 새 메시지)
		5.0f,                 // Duration (5초 동안 표시)
		FColor::Green,        // 글자색
		FString::Printf(TEXT("TotalMass: %.2f, BrokenMass: %.2f"), TotalGeometryMass, CurrentBrokenMass)
	);*/
	
	// 획득 가능한 자원 계산
	int MaxCollectableResourceCount = static_cast<int32>(CurrentBrokenMass / UnitMassPerResource);
	int CollectableResourceCount = MaxCollectableResourceCount - ConsumedResourceCount;
	
	if (CollectableResourceCount > 0)
	{
		HandleRewardItem(CollectableResourceCount);
		
		// 소비한 자원 개수 재설정
		ConsumedResourceCount += CollectableResourceCount;

		// 액터 삭제 조건
		if (CurrentBrokenMass >= (TotalGeometryMass * MassDestructionThresholdRatio))
		{
			// 쓰러지는 나무 사운드 재생
			GeometryCollection->DestroyComponent();
			
			// 상단 메쉬 컴포넌트 중력 적용
			MeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
			MeshComponent->SetSimulatePhysics(true);
			MeshComponent->SetEnableGravity(true);
			
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle,[this]()
			{
				Destroy();
			},3.0f,false);
		}
	}
}

void AEmberTreeBase::InitResource()
{
	if (RewardItemClass)
	{
		CachedItemID = UEmberItemData::Get().FindItemTemplateIDByClass(RewardItemClass);
	}
	
	if (GeometryCollection)
	{
		TotalGeometryMass = GeometryCollection->GetMass();
		UnitMassPerResource = (TotalGeometryMass * MassDestructionThresholdRatio) / MaxResourceCount;
	}
}

void AEmberTreeBase::HandleRewardItem(int32 RewardItemCount)
{
	if (DamageCauser)
	{
		UItemInstance* ItemInstance = NewObject<UItemInstance>();
		ItemInstance->Init(CachedItemID, RewardItemRarity, RewardItemCount);
		
		EventData.OptionalObject = ItemInstance;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DamageCauser, EventTag, EventData);
	}
}

void AEmberTreeBase::SetDynamic_Implementation(FVector Position)
{
	if (GeometryCollection)
	{
		FVector Origin;
		FVector BoxBound;
		float SphereRadius;
		
		UKismetSystemLibrary::GetComponentBounds(GeometryCollection,Origin, BoxBound, SphereRadius);
		GeometryCollection->ApplyKinematicField(SphereRadius, Position);
	}
}
