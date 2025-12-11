// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManagerComponent.h"

#include "GameFlag.h"
#include "InventoryEquipmentManagerComponent.h"
#include "InventoryManagerComponent.h"
#include "ItemInstance.h"
#include "Interaction/Actors/Pickup/EmberInteractionActorBase.h"

UItemManagerComponent::UItemManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UItemManagerComponent::Server_InventoryToEquipment_Implementation(UInventoryManagerComponent* FromInventoryManager, const FIntPoint& FromItemSlotPos, UInventoryEquipmentManagerComponent* ToInventoryEquipmentManager, EEquipmentSlotType ToEquipmentSlotType)
{
	if (HasAuthority() == false)
    	return;

	if (FromInventoryManager == nullptr || ToInventoryEquipmentManager == nullptr)
		return;

	/*if (IsAllowedComponent(FromInventoryManager) == false ||  IsAllowedComponent(ToEquipmentManager) == false)
		return;*/
	
	int32 MovableCount = ToInventoryEquipmentManager->CanMoveOrMergeEquipment(FromInventoryManager, FromItemSlotPos, ToEquipmentSlotType);
	if (MovableCount > 0)
	{
		UItemInstance* RemovedItemInstance = FromInventoryManager->RemoveItem_Unsafe(FromItemSlotPos, MovableCount);
		ToInventoryEquipmentManager->AddEquipment_Unsafe(ToEquipmentSlotType, RemovedItemInstance, MovableCount);
	}
	else
	{
		FIntPoint ToItemSlotPos;
		if (ToInventoryEquipmentManager->CanSwapEquipment(FromInventoryManager, FromItemSlotPos, ToEquipmentSlotType, ToItemSlotPos))
		{
			const int32 FromItemCount = FromInventoryManager->GetItemCount(FromItemSlotPos);
			const int32 ToItemCount = ToInventoryEquipmentManager->GetItemCount(ToEquipmentSlotType);

			UItemInstance* RemovedItemInstanceFrom = FromInventoryManager->RemoveItem_Unsafe(FromItemSlotPos, FromItemCount);
			UItemInstance* RemovedItemInstanceTo = ToInventoryEquipmentManager->RemoveEquipment_Unsafe(ToEquipmentSlotType, ToItemCount);
			FromInventoryManager->AddItem_Unsafe(ToItemSlotPos, RemovedItemInstanceTo, ToItemCount);
			ToInventoryEquipmentManager->AddEquipment_Unsafe(ToEquipmentSlotType, RemovedItemInstanceFrom, FromItemCount);
		}
	}
}

void UItemManagerComponent::Server_InventoryToInventory_Implementation(UInventoryManagerComponent* FromInventoryManager, const FIntPoint& FromItemSlotPos, UInventoryManagerComponent* ToInventoryManager, const FIntPoint& ToItemSlotPos)
{
	if (HasAuthority() == false)
		return;

	if (FromInventoryManager == nullptr || ToInventoryManager == nullptr)
		return;

	if (FromInventoryManager == ToInventoryManager && FromItemSlotPos == ToItemSlotPos)
		return;

	int32 MovableCount = ToInventoryManager->CanMoveOrMergeItem(FromInventoryManager, FromItemSlotPos, ToItemSlotPos);
	if (MovableCount > 0)
	{
		UItemInstance* RemovedItemInstance = FromInventoryManager->RemoveItem_Unsafe(FromItemSlotPos, MovableCount);
		ToInventoryManager->AddItem_Unsafe(ToItemSlotPos, RemovedItemInstance, MovableCount);
	}
}


void UItemManagerComponent::Server_EquipmentToInventory_Implementation(UInventoryEquipmentManagerComponent* FromEquipmentManager, EEquipmentSlotType FromEquipmentSlotType, UInventoryManagerComponent* ToInventoryManager, const FIntPoint& ToItemSlotPos)
{
	if (HasAuthority() == false)
		return;

	if (FromEquipmentManager == nullptr || ToInventoryManager == nullptr)
		return;

	int32 MovableCount = ToInventoryManager->CanMoveOrMergeItem(FromEquipmentManager, FromEquipmentSlotType, ToItemSlotPos);
	if (MovableCount > 0)
	{
		UItemInstance* RemovedItemInstance = FromEquipmentManager->RemoveEquipment_Unsafe(FromEquipmentSlotType, MovableCount);
		ToInventoryManager->AddItem_Unsafe(ToItemSlotPos, RemovedItemInstance, MovableCount);
	}
}

void UItemManagerComponent::Server_EquipmentToEquipment_Implementation(UInventoryEquipmentManagerComponent* FromEquipmentManager, EEquipmentSlotType FromEquipmentSlotType, UInventoryEquipmentManagerComponent* ToEquipmentManager, EEquipmentSlotType ToEquipmentSlotType)
{
	if (HasAuthority() == false)
		return;
	
	if (FromEquipmentManager == nullptr || ToEquipmentManager == nullptr)
		return;

	if (FromEquipmentManager == ToEquipmentManager && FromEquipmentSlotType == ToEquipmentSlotType)
		return;

	int32 MovableCount = ToEquipmentManager->CanMoveOrMergeEquipment(FromEquipmentManager, FromEquipmentSlotType, ToEquipmentSlotType);
	if (MovableCount > 0)
	{
		UItemInstance* RemovedItemInstance = FromEquipmentManager->RemoveEquipment_Unsafe(FromEquipmentSlotType, MovableCount);
		ToEquipmentManager->AddEquipment_Unsafe(ToEquipmentSlotType, RemovedItemInstance, MovableCount);
	}
	else if (ToEquipmentManager->CanSwapEquipment(FromEquipmentManager, FromEquipmentSlotType, ToEquipmentSlotType))
	{
		const int32 FromItemCount = FromEquipmentManager->GetItemCount(FromEquipmentSlotType);
		const int32 ToItemCount = ToEquipmentManager->GetItemCount(ToEquipmentSlotType);
		
		UItemInstance* RemovedItemInstanceFrom = FromEquipmentManager->RemoveEquipment_Unsafe(FromEquipmentSlotType, FromItemCount);
		UItemInstance* RemovedItemInstanceTo = ToEquipmentManager->RemoveEquipment_Unsafe(ToEquipmentSlotType, ToItemCount);
		FromEquipmentManager->AddEquipment_Unsafe(FromEquipmentSlotType, RemovedItemInstanceTo, ToItemCount);
		ToEquipmentManager->AddEquipment_Unsafe(ToEquipmentSlotType, RemovedItemInstanceFrom, FromItemCount);
	}
}

bool UItemManagerComponent::TryPickupItem(AEmberWorldPickupable* PickupableItemActor)
{
	if (HasAuthority() == false)
		return false;

	if (IsValid(PickupableItemActor) == false)
		return false;

	UInventoryManagerComponent* MyInventoryManager = GetMyInventoryManager();
	UInventoryEquipmentManagerComponent* MyInventoryEquipmentManager = GetMyInventoryEquipmentManager();
	if (MyInventoryManager == nullptr || MyInventoryEquipmentManager == nullptr)
		return false;

	const FPickupInfo& PickupInfo = PickupableItemActor->GetPickupInfo();
	if (PickupInfo.ItemInstance == nullptr)
		return false;
	
	TArray<FIntPoint> ToItemSlotPoses;
	TArray<int32> ToItemCounts;
	int32 MovableCount = MyInventoryManager->CanAddItem(PickupInfo.ItemInstance, ToItemSlotPoses, ToItemCounts);
	int32 PickupItemCount = PickupInfo.ItemInstance->GetItemCount();
	if (MovableCount == PickupItemCount)
	{
		for (int32 i = 0; i < ToItemSlotPoses.Num(); i++)
		{
			MyInventoryManager->AddItem_Unsafe(ToItemSlotPoses[i], PickupInfo.ItemInstance, ToItemCounts[i]);
		}
			
		PickupableItemActor->Destroy();
		return true;
	}

	return false;
}


bool UItemManagerComponent::TryAutoPickupItem(UItemInstance* ItemInstance)
{
	if (HasAuthority() == false)
		return false;
	
	if (ItemInstance == nullptr)
		return false;
	
	UInventoryManagerComponent* MyInventoryManager = GetMyInventoryManager();
	if (MyInventoryManager == nullptr)
		return false;
	
	TArray<FIntPoint> ToItemSlotPoses;
	TArray<int32> ToItemCounts;
	int32 MovableCount = MyInventoryManager->CanAddItem(ItemInstance, ToItemSlotPoses, ToItemCounts);
	int32 PickupItemCount = ItemInstance->GetItemCount();
	if (MovableCount == PickupItemCount)
	{
		for (int32 i = 0; i < ToItemSlotPoses.Num(); i++)
		{
			MyInventoryManager->AddItem_Unsafe(ToItemSlotPoses[i], ItemInstance, ToItemCounts[i]);
		}
		return true;
	}

	return false;
}

UInventoryManagerComponent* UItemManagerComponent::GetMyInventoryManager() const
{
	UInventoryManagerComponent* MyInventoryManager = nullptr;
	
	if (AController* Controller = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = Controller->GetPawn())
		{
			MyInventoryManager = Pawn->GetComponentByClass<UInventoryManagerComponent>();
		}
	}

	return MyInventoryManager;
}

UInventoryEquipmentManagerComponent* UItemManagerComponent::GetMyInventoryEquipmentManager() const
{
	UInventoryEquipmentManagerComponent* MyInventoryEquipmentManager = nullptr;
	
	if (AController* Controller = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = Controller->GetPawn())
		{
			MyInventoryEquipmentManager = Pawn->GetComponentByClass<UInventoryEquipmentManagerComponent>();
		}
	}

	return MyInventoryEquipmentManager;
}

