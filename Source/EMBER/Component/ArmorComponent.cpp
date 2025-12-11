#include "ArmorComponent.h"
#include "ItemTemplate.h"
#include "Fragments/ItemFragment_Equipable_Armor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "System/EmberAssetManager.h"

UArmorComponent::UArmorComponent()
{
    SetIsReplicatedByDefault(true);
}

void UArmorComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter == nullptr)
    {
        UE_LOG(LogTemp, Warning, L"Owner Character is null");
        return;
    }
}

bool UArmorComponent::HasOwnerAuthority() const
{
    AActor* Owner = GetOwner();
    return Owner && Owner->HasAuthority();
}

void UArmorComponent::DetermineEquip(const UItemFragment_Equipable_Armor* ArmorFragment, int32 NewItemTemplateID)
{
    UE_LOG(LogTemp, Warning, L"Determine Equip");
    const UItemTemplate& armorItem = UEmberItemData::Get().FindItemTemplateByID(NewItemTemplateID);

    if (ArmorFragment == nullptr)
    {
        UE_LOG(LogTemp, Error, L"ArmorFragment is null");
        return;
    }
    USkeletalMesh* mesh = UEmberAssetManager::GetAssetByPath<USkeletalMesh>(ArmorFragment->ArmorMesh);
    if (mesh == nullptr)
    {
        UE_LOG(LogTemp, Error, L"Mesh is null");
        return;
    }

    // 서버 RPC 호출
    EquipORUnEquip(mesh, ArmorFragment->ArmorType, NewItemTemplateID);
}

void UArmorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // TArray 리플리케이션 등록
    DOREPLIFETIME(UArmorComponent, ArmorDataArray);
}

void UArmorComponent::InitializeArmorForLateJoiners()
{
    // 서버에서만 호출해야 함 (클라이언트에서 호출하면 아무 효과 없음)
    if (!HasOwnerAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializeArmorForLateJoiners: 서버가 아니라서 무시됨"));
        UpdateArmorVisuals();
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("서버에서 모든 클라이언트로 아머 상태 동기화 시작"));

    // 현재 장착된 아머 상태를 모든 클라이언트에게 전파
    MulticastInitializeArmor();
    OwnerCharacter->ForceNetUpdate();
}

void UArmorComponent::MulticastInitializeArmor_Implementation()
{
    // 이 함수는 서버와 모든 클라이언트에서 실행됨

    // 디버깅: 어디서 실행되는지 로그 출력
    UE_LOG(LogTemp, Warning, TEXT("MulticastInitializeArmor 실행: %s"),
        HasOwnerAuthority() ? TEXT("서버") : TEXT("클라이언트"));

    // 서버에서는 이미 상태가 올바르므로 업데이트 불필요
    if (HasOwnerAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("서버에서는 이미 상태가 올바르므로 시각 업데이트 스킵"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("클라이언트: 장비 상태 업데이트 시작 (데이터 개수: %d)"), ArmorDataArray.Num());

    // 클라이언트에서 현재 ArmorDataArray 기반으로 시각적 상태 업데이트
    UpdateArmorVisuals();
}

// 아머 데이터 배열 관리 헬퍼 함수들
void UArmorComponent::AddOrUpdateArmorData(EArmorType ArmorType, int32 ItemTemplateID, const FString& MeshPath)
{
    FArmorData* ExistingData = FindArmorData(ArmorType);

    if (ExistingData)
    {
        // 기존 데이터 업데이트
        ExistingData->ItemTemplateID = ItemTemplateID;
        ExistingData->MeshPath = MeshPath;
    }
    else
    {
        // 새 데이터 추가
        ArmorDataArray.Add(FArmorData(ArmorType, ItemTemplateID, MeshPath));
    }
}

void UArmorComponent::RemoveArmorData(EArmorType ArmorType)
{
    for (int32 i = 0; i < ArmorDataArray.Num(); i++)
    {
        if (ArmorDataArray[i].ArmorType == ArmorType)
        {
            ArmorDataArray.RemoveAt(i);
            break;
        }
    }
}

FArmorData* UArmorComponent::FindArmorData(EArmorType ArmorType)
{
    for (int32 i = 0; i < ArmorDataArray.Num(); i++)
    {
        if (ArmorDataArray[i].ArmorType == ArmorType)
        {
            return &ArmorDataArray[i];
        }
    }
    return nullptr;
}

FString UArmorComponent::ArmorTypeName(EArmorType& ArmorType)
{
    // 기존 코드 유지
    FString ArmorTypeName{};
    switch (ArmorType)
    {
    case EArmorType::Helmet:
        ArmorTypeName = L"Helmet";
        break;
    case EArmorType::Shoulder:
        ArmorTypeName = L"Shoulder";
        break;
    case EArmorType::Cloak:
        ArmorTypeName = L"Cloak";
        break;
    case EArmorType::Chest:
        ArmorTypeName = L"Chest";
        break;
    case EArmorType::Gloves:
        ArmorTypeName = L"Gloves";
        break;
    case EArmorType::Pants:
        ArmorTypeName = L"Pants";
        break;
    case EArmorType::Foot:
        ArmorTypeName = L"Foot";
        break;
    case EArmorType::Count:
    default:
        break;
    }
    return ArmorTypeName;
}

void UArmorComponent::EquipORUnEquip_Implementation(USkeletalMesh* Mesh, EArmorType ArmorType, int32 ItemTemplateID)
{
    // 서버에서만 실행될 코드
    if (HasOwnerAuthority())
    {
        // 로그 추가
        UE_LOG(LogTemp, Warning, TEXT("서버에서 장비 장착 실행 - 아머 타입: %s"), *ArmorTypeName(ArmorType));

        // 기존 장착 로직은 유지
        USkeletalMeshComponent* com = OwnerCharacter->GetMesh();
        TArray<USceneComponent*> child;
        com->GetChildrenComponents(false, child);
        USkeletalMeshComponent* childMesh{};

        for (USceneComponent* chi : child)
        {
            FString armorTypeName = ArmorTypeName(ArmorType);
            if (chi->GetName().Contains(armorTypeName) == true)
            {
                childMesh = Cast<USkeletalMeshComponent>(chi);
                if (childMesh != nullptr && childMesh->GetSkeletalMeshAsset() == nullptr)
                {
                    // 장비 장착 로직
                    childMesh->SetSkeletalMesh(Mesh);
                    childMesh->SetLeaderPoseComponent(com);
                    childMesh->SetAnimInstanceClass(com->GetAnimClass());
                    childMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
                    childMesh->SetVisibility(true);
                    childMesh->UpdateLeaderBoneMap();
                    TMap<EArmorType, int32> temp{};
                    temp.Add(ArmorType, ItemTemplateID);
                    AttachArmors.Add(childMesh, temp);

                    // 리플리케이션 데이터 업데이트
                    if (Mesh != nullptr)
                    {
                        AddOrUpdateArmorData(ArmorType, ItemTemplateID, Mesh->GetPathName());
                    }

                    break;
                }
                else if (childMesh != nullptr && childMesh->GetSkeletalMeshAsset() != nullptr)
                {
                    if (AttachArmors[childMesh].Contains(ArmorType) == true)
                    {
                        if (AttachArmors[childMesh][ArmorType] == ItemTemplateID)
                        {
                            // 장비 해제 로직
                            Mesh = nullptr;
                            AttachArmors.Remove(childMesh);

                            // 리플리케이션 데이터 업데이트
                            RemoveArmorData(ArmorType);
                        }
                        else
                        {
                            // 다른 장비로 교체
                            AttachArmors[childMesh][ArmorType] = ItemTemplateID;

                            // 리플리케이션 데이터 업데이트
                            if (Mesh != nullptr)
                            {
                                AddOrUpdateArmorData(ArmorType, ItemTemplateID, Mesh->GetPathName());
                            }
                        }
                        childMesh->SetSkeletalMesh(Mesh);
                    }
                }
            }
        }

        // 로그 추가
        UE_LOG(LogTemp, Warning, TEXT("서버에서 장비 장착 완료 - 클라이언트에 리플리케이션 예정"));
    }
}

// OnRep 콜백 함수 구현
void UArmorComponent::OnRep_ArmorDataArray()
{
    // 클라이언트에서만 실행될 시각적 업데이트 로직
    UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 OnRep_ArmorDataArray 콜백 호출됨"));

    // 시각적 업데이트 함수 호출
    UpdateArmorVisuals();
}

// 시각적 업데이트 함수 구현
void UArmorComponent::UpdateArmorVisuals()
{
    if (OwnerCharacter == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("UpdateArmorVisuals: OwnerCharacter가 없음"));
        return;
    }

    // 서버에서는 이미 업데이트되어 있으므로 클라이언트에서만 실행
    if (HasOwnerAuthority())
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 장비 시각 업데이트 시작"));

    USkeletalMeshComponent* mainMesh = OwnerCharacter->GetMesh();
    TArray<USceneComponent*> childComponents;
    mainMesh->GetChildrenComponents(false, childComponents);

    // 모든 아머 타입에 대해 처리
    for (const FArmorData& ArmorData : ArmorDataArray)
    {
        EArmorType armorType = ArmorData.ArmorType;
        int32 itemTemplateID = ArmorData.ItemTemplateID;
        FString armorTypeName = ArmorTypeName(armorType);

        // 메시 로드
        USkeletalMesh* armorMesh = LoadObject<USkeletalMesh>(nullptr, *ArmorData.MeshPath);
        if (!armorMesh)
        {
            UE_LOG(LogTemp, Error, TEXT("메시 로드 실패: %s"), *ArmorData.MeshPath);
            continue;
        }

        // 해당 타입의 컴포넌트 찾기
        USkeletalMeshComponent* targetComponent = nullptr;
        for (USceneComponent* child : childComponents)
        {
            if (child->GetName().Contains(armorTypeName))
            {
                targetComponent = Cast<USkeletalMeshComponent>(child);
                break;
            }
        }

        // 컴포넌트에 메시 설정
        if (targetComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("클라이언트: 아머 타입 %s 메시 업데이트"), *armorTypeName);
            targetComponent->SetSkeletalMesh(armorMesh);
            targetComponent->SetLeaderPoseComponent(mainMesh);
            targetComponent->SetAnimInstanceClass(mainMesh->GetAnimClass());
            targetComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
            targetComponent->SetVisibility(true);
            targetComponent->UpdateLeaderBoneMap();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("클라이언트: 아머 타입 %s의 컴포넌트를 찾을 수 없음"), *armorTypeName);
        }
    }

    // 장착 해제된 아머 처리 (ArmorDataArray에 없는 타입들)
    for (USceneComponent* child : childComponents)
    {
        USkeletalMeshComponent* meshComp = Cast<USkeletalMeshComponent>(child);
        if (meshComp && meshComp != mainMesh)
        {
            bool isEquipped = false;
            for (FArmorData& ArmorData : ArmorDataArray)
            {
                FString armorTypeName = ArmorTypeName(ArmorData.ArmorType);
                if (child->GetName().Contains(armorTypeName))
                {
                    isEquipped = true;
                    break;
                }
            }

            // 장착 중이 아니면 메시 제거
            if (!isEquipped && meshComp->GetSkeletalMeshAsset() != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("클라이언트: 컴포넌트 %s 메시 제거"), *child->GetName());
                meshComp->SetSkeletalMesh(nullptr);
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 장비 시각 업데이트 완료"));
}