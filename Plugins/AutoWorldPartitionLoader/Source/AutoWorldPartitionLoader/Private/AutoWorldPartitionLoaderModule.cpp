#include "Modules/ModuleManager.h"

class FAutoWorldPartitionLoaderModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        UE_LOG(LogTemp, Warning, TEXT("[AutoWorldPartitionLoader] Module Startup."));
    }

    virtual void ShutdownModule() override
    {
        UE_LOG(LogTemp, Warning, TEXT("[AutoWorldPartitionLoader] Module Shutdown."));
    }
};

IMPLEMENT_MODULE(FAutoWorldPartitionLoaderModule, AutoWorldPartitionLoader)
