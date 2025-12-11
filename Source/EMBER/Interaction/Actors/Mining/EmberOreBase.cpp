// Fill out your copyright notice in the Description page of Project Settings.

#include "EmberOreBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "Field/FieldSystemActor.h"
#include "GameInfo/GameplayTags.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interaction/Physics/WeaponImpactField.h"
#include "kismet/GameplayStatics.h"
#include "UI/Data/EmberItemData.h"

AEmberOreBase::AEmberOreBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	
	RequiredTags.AddTag(EmberGameplayTags::Ability_Mining);

	AnchorField = CreateDefaultSubobject<UChildActorComponent>("AnchorField");
	AnchorField->SetHiddenInGame(true);
	AnchorField->SetupAttachment(GetRootComponent());
	
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollection");
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetNotifyBreaks(true);
	GeometryCollection->SetCollisionProfileName(TEXT("Mineable"));
	GeometryCollection->CanCharacterStepUpOn = ECB_No;
	GeometryCollection->SetupAttachment(GetRootComponent());

	EventTag = EmberGameplayTags::GameplayEvent_Reward_Item;
}

void AEmberOreBase::BeginPlay()
{
	Super::BeginPlay();
	
	AFieldSystemActor* FieldSystemActor = Cast<AFieldSystemActor>(AnchorField->GetChildActor());
	GeometryCollection->InitializationFields.Add(FieldSystemActor);
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ThisClass::HandleBreakEvent);
	OnTakePointDamage.AddDynamic(this, &ThisClass::HandlePointDamage);
	
	InitResource();
}

void AEmberOreBase::HandlePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
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
		float ForceMagnitude = 37000.f;
		float SphereScaleMultiplier = 2.f;
		
		WeaponImpact->Init(ForceMagnitude, SphereScaleMultiplier);
		UGameplayStatics::FinishSpawningActor(WeaponImpact, SpawnTransform);
	}

	DamageCauser = InDamageCauser;
}

void AEmberOreBase::HandleBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	if (HasAuthority() == false)
		return;

	// 부서진 암석의 개수
	CurrentBrokenElements += 1;
	
	// TODO: 꺠지는 사운드 재생
	
	// 획득 가능한 자원 계산
	int MaxCollectableResourceCount = static_cast<int32>(CurrentBrokenElements / UnitElementPerResource);
	int CollectableResourceCount = MaxCollectableResourceCount - ConsumedResourceCount;
	
	if (CollectableResourceCount > 0)
	{
		HandleRewardItem(CollectableResourceCount);
		
		// 소비한 자원 개수 재설정
		ConsumedResourceCount += CollectableResourceCount;

		// 자원을 모두 소비한 경우에는 액터 삭제
		if (ConsumedResourceCount >= MaxResourceCount)
		{
			Destroy();
		}
	}
}

void AEmberOreBase::InitResource()
{
	if (RewardItemClass)
	{
		ItemID = UEmberItemData::Get().FindItemTemplateIDByClass(RewardItemClass);
	}
	
	if (GeometryCollection)
	{
		int32 NumElements = GeometryCollection->GetNumElements("Transform");
		UnitElementPerResource = NumElements / MaxResourceCount;
		CurrentBrokenElements = UnitElementPerResource * 2;
	}
}

void AEmberOreBase::HandleRewardItem(int32 RewardItemCount)
{
	if (DamageCauser)
	{
		UItemInstance* ItemInstance = NewObject<UItemInstance>();
		ItemInstance->Init(ItemID, RewardItemRarity, RewardItemCount);
		
		EventData.OptionalObject = ItemInstance;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DamageCauser, EventTag, EventData);
	}
}

void AEmberOreBase::SetDynamic_Implementation(FVector Position)
{
	float Radius = 220.f;
	
	GeometryCollection->ApplyKinematicField(Radius, Position);
}
