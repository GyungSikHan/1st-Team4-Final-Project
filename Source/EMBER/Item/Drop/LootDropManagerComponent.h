#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Item/Drop/LootTable.h"
#include "LootDropManagerComponent.generated.h"

class UDataTable;
class APickupItemActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API ULootDropManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULootDropManagerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnMonsterDiedMessageReceived(FGameplayTag Channel, const FMonsterDiedMessage& Message);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Settings")
	TObjectPtr<UDataTable> MasterLootTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Settings")
	TSubclassOf<APickupItemActor> PickupItemActorClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot Settings", meta = (Categories = "Event.Monster"))
	FGameplayTag MonsterDiedChannel;

private:
	FGameplayMessageListenerHandle MessageListenerHandle;
};