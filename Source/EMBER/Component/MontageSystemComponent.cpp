#include "MontageSystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

UMontageSystemComponent::UMontageSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMontageSystemComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter == nullptr)
    {
        UE_LOG(LogTemp, Error, L"Owner Character is null");
        return;
    }
 
    MeshComp = OwnerCharacter->FindComponentByClass<USkeletalMeshComponent>();
    if (MeshComp)
    {
        AnimInstance = MeshComp->GetAnimInstance();
    }
 

    TArray<FMontagesData*> datas;
    if (DataTable == nullptr)
    {
        UE_LOG(LogTemp, Error, L"DataTable is null");
        return;
    }
    DataTable->GetAllRows<FMontagesData>("", datas);
    for (int32 i = 0; i < (int32)EStateType::Max; i++)
    {
        for (FMontagesData* data : datas)
        {
            Datas[(int32)data->Type] = data;
        }//for(data)
    }
}

void UMontageSystemComponent::MulticastPlayMontage_Implementation(UAnimMontage* Montage, float PlayRate, FName SectionName)
{
    if (AnimInstance)
    {
        float Duration = AnimInstance->Montage_Play(Montage,PlayRate);

        if (Duration > 0.f)
        {
            // Start at a given Section.
            if (SectionName != NAME_None)
            {
                AnimInstance->Montage_JumpToSection(SectionName, Montage);
            }
        }
    }
}

void UMontageSystemComponent::ServerPlayMontage_Implementation(UAnimMontage* Montage, float PlayRate, FName SectionName)
{
    if (AnimInstance)
    {
        MulticastPlayMontage(Montage, PlayRate);
    }
}

bool UMontageSystemComponent::ServerPlayMontage_Validate(UAnimMontage* Montage, float PlayRate, FName SectionName)
{
    return true;  // 여기에 검증 로직 추가 가능
}

void UMontageSystemComponent::PlayMontage(UAnimMontage* Montage, float PlayRate, FName SectionName)
{
    if (!Montage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Montage is null!"));
        return;
    }
    
    if (OwnerCharacter == nullptr)
    {
        UE_LOG(LogTemp, Error, L"Owner Character is null");
        return;
    }
    
    if (OwnerCharacter->HasAuthority())
    {
        // 서버인 경우 바로 호출
        MulticastPlayMontage(Montage, PlayRate, SectionName);
    }
    else
    {
        // 클라이언트인 경우 서버에 요청
        ServerPlayMontage(Montage, PlayRate, SectionName);
    }

}

void UMontageSystemComponent::PlayMontage(EStateType InType)
{
    if (OwnerCharacter == nullptr)
    {
        UE_LOG(LogTemp, Error, L"Owner Character is null");
        return;
    }

    FMontagesData* data = Datas[(int32)InType];

    if (data == nullptr || data->Montage == nullptr)
    {
        GLog->Log(ELogVerbosity::Error, "None montages data");

        return;
    }
    
    if (OwnerCharacter->HasAuthority())
    {
        // 서버인 경우 바로 호출
        MulticastPlayMontage(data->Montage, data->PlayRate);
    }
    else
    {
        // 클라이언트인 경우 서버에 요청
        ServerPlayMontage(data->Montage,data->PlayRate);
    }
}