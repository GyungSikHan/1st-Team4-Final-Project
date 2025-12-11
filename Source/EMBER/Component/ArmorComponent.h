#pragma once
#include "CoreMinimal.h"
#include "GameFlag.h"
#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"

// �Ƹ� �����͸� ������ ����ü ����
USTRUCT()
struct FArmorData
{
    GENERATED_BODY()
public:
    UPROPERTY()
    EArmorType ArmorType;

    UPROPERTY()
    int32 ItemTemplateID;

    UPROPERTY()
    FString MeshPath;

    FArmorData() : ArmorType(EArmorType::Count), ItemTemplateID(0), MeshPath("") {}

    FArmorData(EArmorType InType, int32 InID, const FString& InPath)
        : ArmorType(InType), ItemTemplateID(InID), MeshPath(InPath) {
    }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMBER_API UArmorComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UArmorComponent();
protected:
    virtual void BeginPlay() override;
public:
    void DetermineEquip(const class UItemFragment_Equipable_Armor* ArmorFragment, int32 NewItemTemplateID);
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void InitializeArmorForLateJoiners();

public:
	UFUNCTION(NetMulticast, Reliable)
    void MulticastInitializeArmor();

private:
    UFUNCTION(Reliable, Server)
    void EquipORUnEquip(USkeletalMesh* Mesh, EArmorType ArmorType, int32 ItemTemplateID);


    // OnRep �ݹ� �Լ� ����
    UFUNCTION()
    void OnRep_ArmorDataArray();

private:
    // Ŭ���̾�Ʈ���� ����� �ð��� ������Ʈ �Լ�
    void UpdateArmorVisuals();

    // �Ƹ� ������ �迭 ���� ���� �Լ�
    void AddOrUpdateArmorData(EArmorType ArmorType, int32 ItemTemplateID, const FString& MeshPath);
    void RemoveArmorData(EArmorType ArmorType);
    FArmorData* FindArmorData(EArmorType ArmorType);
    FString ArmorTypeName(EArmorType& ArmorType);

    // ���� ������ ������ Ȯ���ϴ� ���� �Լ�
    bool HasOwnerAuthority() const;


private:
    // ���ø����̼ǵ� ������ - TMap ��� TArray ���
    UPROPERTY(ReplicatedUsing = OnRep_ArmorDataArray)
    TArray<FArmorData> ArmorDataArray;
private:
    ACharacter* OwnerCharacter;
    TMap<USkeletalMeshComponent*, TMap<EArmorType, int32>> AttachArmors;
};