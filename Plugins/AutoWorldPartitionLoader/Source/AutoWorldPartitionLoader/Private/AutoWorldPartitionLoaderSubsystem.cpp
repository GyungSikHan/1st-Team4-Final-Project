#include "AutoWorldPartitionLoaderSubsystem.h"
#include "Editor.h"
#include "Engine/World.h"
#include "WorldPartition/WorldPartition.h"

void UAutoWorldPartitionLoaderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Warning, TEXT("[AutoWorldPartitionLoader] Subsystem Initialized."));

    LoadAllWorldPartitionCells();

    FEditorDelegates::OnMapOpened.AddUObject(this, &UAutoWorldPartitionLoaderSubsystem::OnMapOpened);
}

void UAutoWorldPartitionLoaderSubsystem::OnMapOpened(const FString& Filename, bool bAsTemplate)
{
    UE_LOG(LogTemp, Warning, TEXT("[AutoWorldPartitionLoader] Map opened: %s"), *Filename);
    LoadAllWorldPartitionCells();
}

void UAutoWorldPartitionLoaderSubsystem::LoadAllWorldPartitionCells()
{
    if (!GEditor) return;

    UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
    if (!EditorWorld) return;

    UE_LOG(LogTemp, Warning, TEXT("[AutoWorldPartitionLoader] Editor World Found."));

    UWorldPartition* WorldPartition = EditorWorld->GetWorldPartition();
    if (!WorldPartition)
    {
        UE_LOG(LogTemp, Error, TEXT("[AutoWorldPartitionLoader] No World Partition found."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("[AutoWorldPartitionLoader] Using Reflection to load all cells."));

    UClass* SubsystemClass = FindObject<UClass>(nullptr, TEXT("/Script/WorldPartitionEditor.WorldPartitionEditorSubsystem"));
    if (SubsystemClass)
    {
        UObject* SubsystemObj = GEditor->GetEditorSubsystemBase(SubsystemClass);
        if (SubsystemObj)
        {
            UFunction* LoadAllCellsFunction = SubsystemClass->FindFunctionByName("LoadAllCells");
            if (LoadAllCellsFunction)
            {
                struct FLoadAllCellsParams { UWorldPartition* WorldPartition; };
                FLoadAllCellsParams Params;
                Params.WorldPartition = WorldPartition;

                SubsystemObj->ProcessEvent(LoadAllCellsFunction, &Params);
            }
        }
    }
}
