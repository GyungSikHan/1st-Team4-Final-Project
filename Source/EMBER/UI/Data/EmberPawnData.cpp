// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberPawnData.h"

#include "System/EmberAssetManager.h"

UEmberPawnData::UEmberPawnData(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UEmberPawnData& UEmberPawnData::Get()
{
	return UEmberAssetManager::Get().GetPawnData();
}
