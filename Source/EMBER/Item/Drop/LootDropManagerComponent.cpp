#include "Item/Drop/LootDropManagerComponent.h"
#include "Item/Drop/LootTable.h"
#include "Engine/DataTable.h"
#include "Item/ItemTemplate.h"
#include "Item/Drop/PickupItemActor.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"

ULootDropManagerComponent::ULootDropManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MasterLootTable = nullptr;
	PickupItemActorClass = nullptr;
}

void ULootDropManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		if (MonsterDiedChannel.IsValid())
		{
			if (GetWorld() == nullptr)
				return;
			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageListenerHandle = MessageSubsystem.RegisterListener(MonsterDiedChannel, this, &ULootDropManagerComponent::OnMonsterDiedMessageReceived);
			UE_LOG(LogTemp, Log, TEXT("LootDropManagerComponent: Listening for monster deaths on channel %s"), *MonsterDiedChannel.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LootDropManagerComponent: MonsterDiedChannel GameplayTag is not set in the component's defaults!"));
		}
	}
}

void ULootDropManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		if (MessageListenerHandle.IsValid())
		{
			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSubsystem.UnregisterListener(MessageListenerHandle);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void ULootDropManagerComponent::OnMonsterDiedMessageReceived(FGameplayTag Channel, const FMonsterDiedMessage& Message)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	if (!MasterLootTable || !PickupItemActorClass) return;

	const FMonsterLootProfile* LootProfile = MasterLootTable->FindRow<FMonsterLootProfile>(Message.MonsterID, TEXT("OnMonsterDiedMessageReceived"));
	if (!LootProfile) return;

	UWorld* World = GetWorld();
	if (!World) return;

	TArray<FLootResultData> FinalLootResults;

	for (const FLootDropItem& DropRule : LootProfile->LootDrops)
	{
		const float RandomRollForDrop = FMath::FRand();
		if (RandomRollForDrop < DropRule.DropChance)
		{
			if (DropRule.ItemTemplateClass && DropRule.QuantityRange.Y > 0 && DropRule.PossibleRarities.Num() > 0)
			{
				const int32 QuantityToDrop = FMath::RandRange(DropRule.QuantityRange.X, DropRule.QuantityRange.Y);

				if (QuantityToDrop > 0)
				{
					EItemRarity FinalRarity = EItemRarity::Common;
					float TotalWeight = 0.f;
					for (const FRarityDropInfo& RarityInfo : DropRule.PossibleRarities)
					{
						TotalWeight += RarityInfo.Weight;
					}
					
					if (TotalWeight > 0.f)
					{
						float RandomRollForRarity = FMath::FRand() * TotalWeight;
						for (const FRarityDropInfo& RarityInfo : DropRule.PossibleRarities)
						{
							if (RandomRollForRarity < RarityInfo.Weight)
							{
								FinalRarity = RarityInfo.Rarity;
								break;
							}
							RandomRollForRarity -= RarityInfo.Weight;
						}
					}

					FLootResultData ResultData;
					ResultData.ItemTemplateClass = DropRule.ItemTemplateClass;
					ResultData.Quantity = QuantityToDrop;
					ResultData.Rarity = FinalRarity;
					FinalLootResults.Add(ResultData);
				}
			}
		}
	}

	if (!FinalLootResults.IsEmpty())
	{
		const FVector SpawnLocation = Message.DeathLocation + FVector(0, 0, 50.0f);
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		
		FTransform SpawnTransform = FTransform::Identity;
		SpawnTransform.SetLocation(SpawnLocation);

		APickupItemActor* NewPouch = GetWorld()->SpawnActorDeferred<APickupItemActor>(
			PickupItemActorClass,
			SpawnTransform,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			ESpawnActorScaleMethod::MultiplyWithRoot
		);
	
		if (NewPouch)
		{
			NewPouch->InitializeLootDrop(FinalLootResults[0]);
			UE_LOG(LogTemp, Log, TEXT("[SERVER] LootDropManager: Spawned loot pouch for %s with %d item types."), *Message.MonsterID.ToString(), FinalLootResults.Num());
		}

		NewPouch->FinishSpawning(FTransform::Identity, true);
	}
}