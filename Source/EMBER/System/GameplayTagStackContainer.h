// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayTagStackContainer.generated.h"

struct FNetDeltaSerializeInfo;
struct FGameplayTagStackContainer;

USTRUCT(BlueprintType)
struct FEmberGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FEmberGameplayTagStack() {}
	FEmberGameplayTagStack(FGameplayTag InTag, int32 InStackCount): Tag(InTag), StackCount(InStackCount) { }

public:
	const FGameplayTag& GetStackTag() const { return Tag; }
	int32 GetStackCount() const { return StackCount; }

private:
	friend FGameplayTagStackContainer;
	
	UPROPERTY()
	FGameplayTag Tag;

	TMap<FGameplayTag, int32> Stack;
	
	UPROPERTY()
	int32 StackCount = 0;
};


USTRUCT()
struct FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()
	
public:
	//~FFastArraySerializer Overrides
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer

public:
	void AddOrRemoveStack(FGameplayTag Tag, int32 StackCount);
	void AddStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStack(FGameplayTag Tag, int32 StackCount);

public:
	int32 GetStackCount(FGameplayTag Tag) const;
	const TArray<FEmberGameplayTagStack>& GetStacks() const { return Stacks; }
	
private:
	UPROPERTY()
	TArray<FEmberGameplayTagStack> Stacks;

	TMap<FGameplayTag, int32> TagToCountMap;
};