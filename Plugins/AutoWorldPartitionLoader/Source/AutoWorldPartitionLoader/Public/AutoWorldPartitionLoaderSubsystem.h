#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "AutoWorldPartitionLoaderSubsystem.generated.h"

UCLASS()
class UAutoWorldPartitionLoaderSubsystem : public UEditorSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    void LoadAllWorldPartitionCells();
    void OnMapOpened(const FString& Filename, bool bAsTemplate);
};
