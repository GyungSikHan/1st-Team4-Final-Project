// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagStackContainer.h"

bool FGameplayTagStackContainer::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FEmberGameplayTagStack, FGameplayTagStackContainer>(Stacks, DeltaParms, *this);
}

void FGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

void FGameplayTagStackContainer::AddOrRemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (Tag.IsValid() == false || StackCount == 0)
		return;

	(StackCount > 0) ? AddStack(Tag, StackCount) : RemoveStack(Tag, FMath::Abs(StackCount));
}

void FGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	if (Tag.IsValid() == false || StackCount <= 0)
		return;

	for (FEmberGameplayTagStack& Stack : Stacks)
	{
		if (Stack.Tag == Tag)
		{
			const int32 NewCount = Stack.StackCount + StackCount;
			Stack.StackCount = NewCount;
			TagToCountMap[Tag] = NewCount;
		}
	}

	FEmberGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
	MarkItemDirty(NewStack);
	TagToCountMap.Emplace(Tag, StackCount);
}

void FGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (Tag.IsValid() == false || StackCount <= 0)
		return;

	for (auto It = Stacks.CreateIterator(); It; ++It)
	{
		FEmberGameplayTagStack& Stack = *It;

		if (Stack.Tag == Tag)
		{
			if (Stack.StackCount <= StackCount)
			{
				It.RemoveCurrent();
				TagToCountMap.Remove(Tag);
				MarkArrayDirty();
			}
			else
			{
				const int32 NewCount = Stack.StackCount - Stack.StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
				MarkItemDirty(Stack);
			}
			return;
		}
	}
}

int32 FGameplayTagStackContainer::GetStackCount(FGameplayTag Tag) const
{
	return TagToCountMap.FindRef(Tag);
}
