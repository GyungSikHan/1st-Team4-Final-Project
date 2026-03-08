# **EMBER: The Eternal Blizzard**
<table>
	<tr>
		<td align="center">
			<img src="Image/image3.png" width="1000"><br>
		</td>
	</tr>
</table>

## 📌 Table of Contents

- [⚡ 30초 요약 (TL;DR)](#-30초-요약-tldr)
- [🎥 게임 플레이 영상](#-게임-플레이-영상)
- [📌 프로젝트 소개](#-프로젝트-소개)
- [🎮 게임 개발](#-게임-개발)
- [🎮 포트폴리오 링크](#-포트폴리오-링크)
- [🖼 In-Game Screenshot](#-in-game-screenshot)
- [👨‍💻 My Key Contributions](#-my-key-contributions)
- [Troubleshooting](#troubleshooting)
- [Retrospective (느낀점)](#retrospective-느낀점)

# ⚡ 30초 요약 (TL;DR)
- **EMBER: The Eternal Blizzard**: Unreal Engine 5.5 기반 6인 팀 Survival Action 프로젝트
- **내 핵심 구현**
  - AI 전투 로직(Combat/Weapon/Damage/Sound) 설계 및 구현
  - Main UI 에셋 연동(메인 메뉴 -> 게임 진입 흐름 연결)
  - 플레이어 방어구 장착 시스템 구현(멀티 전환 대비 네트워크 동기화)
- **가장 어려웠던 점 -> 해결**
  - Dragon AI 공격 후 상태가 멈추는 문제를 `Task 완료 시점` 이슈로 분석
  - BT Task를 `InProgress`로 유지하고 몽타주 종료 콜백에서 `FinishLatentTask`로 완료 처리
- **바로 보기**
  - 📌 시스템 설명/코드 링크: 아래 `My Key Contributions` 섹션 참고

# 🎥 게임 플레이 영상
<p align="center">
  <a href="https://youtu.be/q2ws313NTcg">
    <img src="Image/image.png" width="1000"></br>
    <em>트레일러</em>
  </a>
</p>
<p align="center">
  <a href="https://youtu.be/1DuNwBaC0Xg">
    <img src="Image/image.png" width="1000"></br>
    <em>시연 영상</em>
  </a>
</p>

# 📌 프로젝트 소개

"**EMBER: The Eternal Blizzard**"는 언리얼 엔진 기반으로 제작된 Survival Action 게임으로, 얼어붙은 세상을 구하기 위해 몬스터를 물리치고 영혼의 불꽃을 찾아 모험을 떠나는 세계관을 담고 있습니다.
플레이어는 극한 환경에서 생존하며 전투, 탐험, 성장 루프를 반복해 목표를 달성합니다.

# 🎮 게임 개발
>
> - **인원**: 6인
> - **기간**: 25.05 ~ 25.07
> - **목적**: 언리얼 엔진의 **Gameplay Framework**와 **Component 기반 구조**를 이해하고, C++과 Blueprint 기반으로 AI 구현 및 Main UI Template를 적용하여 게임 진입 흐름을 연결하는 것을 목표로 함.
> - **기술**: C++, Unreal Engine 5.5, Blueprint, Git, Slack, Rider/Visual Studio
</aside>

# 🎮 포트폴리오 링크
https://drive.google.com/file/d/1zo_iDcPDlLVG9eGryJzAW-Y4Bwr19yAZ/view?usp=sharing

# 브로셔
https://www.notion.so/1-4-2246365cac3f816ab542fa9e75a4ac7e?source=copy_link

## 🖼 In-Game Screenshot
<table>
    <tr>
        <td align="center">
             <img src="Image/image2.png" width="1000"><br>
		</td>
</table>
<table>
    <tr>
        <td align="center">
             <img src="Image/image4.gif" width="1000"><br>
		</td>
</table>

---

# **👨‍💻 My Key Contributions**

## AI
### ✔ 설계 의도
- AI 전투를 `Combat / Weapon / Damage / Sound / Behavior`로 분리해 유지보수성과 확장성을 확보

### ✔ 구현 내용
#### ↳ [AI Combat System](Source/EMBER/GameInfo/GameData.cpp)
- 기본 공격 시스템: 공격 데이터에 몽타주를 넣으면 공통 경로에서 재생되도록 구현
- 멀티 전환 대비: MontageSystemComponent에서 Server/NetMulticast 경로로 몽타주 동기화 처리

**관련 코드 1: [GameData.cpp](Source/EMBER/GameInfo/GameData.cpp)**
```cpp
void FAttackData::DoAction(ACharacter* InOwner)
{
    UMontageSystemComponent* montage = Cast<UMontageSystemComponent>(InOwner->GetComponentByClass(UMontageSystemComponent::StaticClass()));
    if(montage == nullptr || Montages == nullptr)
        return;

    montage->PlayMontage(Montages, PlayRate);
}
```
설명: 공격 데이터에 설정된 몽타주가 동일 로직으로 실행되어 AI별 기본 공격 재사용이 가능해짐.

**관련 코드 2: [BTT_DragonAttack.cpp](Source/EMBER/AI/Task/BTT_DragonAttack.cpp)**
```cpp
EBTNodeResult::Type UBTT_DragonAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	DragonAnim->Montage_SetEndDelegate(EndDelegate, RangedAttackMontage);
	return EBTNodeResult::InProgress;
}
```
설명: BT Task를 `InProgress`로 유지하고 몽타주 종료 시점에 동기화해 공격 후 상태 멈춤을 방지.

**관련 코드 3: [MontageSystemComponent.h](Source/EMBER/Component/MontageSystemComponent.h), [MontageSystemComponent.cpp](Source/EMBER/Component/MontageSystemComponent.cpp)**
```cpp
UFUNCTION(NetMulticast, Reliable)
void MulticastPlayMontage(UAnimMontage* Montage,float PlayRate = 1.f,FName SectionName = NAME_None);

UFUNCTION(Server, Reliable, WithValidation)
void ServerPlayMontage(UAnimMontage* Montage, float PlayRate = 1.f, FName SectionName = NAME_None);
```
설명: 싱글 출시로 전환됐지만, 멀티 전환 대비로 서버 요청/멀티캐스트 동기화 경로를 준비.

#### ↳ [AI Weapon](Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp)

**관련 코드 1: [CAI_Weapon.cpp](Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp)**
```cpp
for (USceneComponent* child : children)
{
	UShapeComponent* shape = Cast<UShapeComponent>(child);
	if(shape != nullptr)
	{
		shape->OnComponentBeginOverlap.AddDynamic(this,&ACAI_Weapon::OnComponentBeginOverlap);
		shape->OnComponentEndOverlap.AddDynamic(this, &ACAI_Weapon::OnComponentEndOverlap);
		Collisions.Add(child);
	}
}
OffCollision();
```
설명: 전용 충돌체를 동적으로 등록하고 기본값을 비활성화해 공격 구간에서만 판정되도록 구성.

**관련 코드 2: [CAI_Weapon.h](Source/EMBER/AI/AIWeapon/CAI_Weapon.h), [CAI_Weapon.cpp](Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp)**
```cpp
UFUNCTION(BlueprintCallable, Category = "Attach")
void AttachTo(FName InSocketName);
```
```cpp
void ACAI_Weapon::AttachTo(FName InSocketName)
{
	if(collision->GetName() == InSocketName.ToString())
		collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}
```
설명: 블루프린트에서 C++ Attach 함수를 호출해 소켓 부착을 제어할 수 있도록 연동.

#### ↳ [AI Damage System](Source/EMBER/GameInfo/GameData.h)

**관련 코드 1: [GameData.h](Source/EMBER/GameInfo/GameData.h)**
```cpp
USTRUCT(BlueprintType)
struct FDamageData
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Damage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FEffectData HitEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FSound2D HitSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TObjectPtr<UAnimMontage> Montages;
    void SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther);
};
```
설명: 데미지 값과 피격 연출 데이터를 GameData로 묶어 데이터 기반 밸런싱이 가능해짐.

**관련 코드 2: [GameData.cpp](Source/EMBER/GameInfo/GameData.cpp)**
```cpp
void FDamageData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
    if(InAttacker->HasAuthority() == false)
        return;

    FActionDamageEvent e;
    e.DamageData = this;
    InOther->TakeDamage(Damage, e, InAttacker->GetController(), InAttackCauser);
}
```
설명: 서버 권한에서만 데미지를 반영해 네트워크 환경에서 피해 적용 일관성을 유지.

**관련 코드 2-1: [BaseAI.h](Source/EMBER/AI/Base/BaseAI.h), [BaseAI.cpp](Source/EMBER/AI/Base/BaseAI.cpp)**
```cpp
UPROPERTY(ReplicatedUsing = "OnRep_Hitted")
FDamagesData DamageData;

UFUNCTION(NetMulticast, Reliable)
void MulticastHitted(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

DOREPLIFETIME(ABaseAI, DamageData);
```
설명: 데미지 적용 후 피격 데이터 복제/멀티캐스트로 상태 반영 경로를 구성(멀티 전환 대비).

**관련 코드 3: [CAI_Weapon.cpp](Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp)**
```cpp
Hitted.AddUnique(other);
HitDatas[CurrAttackIndex].SendDamage(OwnerCharacter, this, other);
```
설명: 중복 타격 방지 후 공격 인덱스별 데미지 데이터를 적용.

#### ↳ [AI Sound System](Source/EMBER/AI/Notify/CAnimNotify_AISound.cpp)

**관련 코드 1: [CAnimNotify_AISound.cpp](Source/EMBER/AI/Notify/CAnimNotify_AISound.cpp)**
```cpp
void UCAnimNotify_AISound::Notify(...)
{
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(MeshComp->GetOwner());
	if (ai == nullptr) return;
	ai->PlaySound(SoundType);
}
```
설명: Anim Notify에서 SoundType을 전달해 AI 상태/행동별 사운드를 분기.

**관련 코드 2: [BaseAI.cpp](Source/EMBER/AI/Base/BaseAI.cpp)**
```cpp
UGameplayStatics::SpawnSoundAtLocation(
	GetWorld(), AISounds[(int32)InSoundType], GetActorLocation(),
	FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, SoundAttenuation
);
```
설명: SoundAttenuation 기반 3D 사운드 재생으로, 맵 전체 재생 문제를 막고 거리 기반 감쇠를 적용.

#### ↳ [AI 구조 개선](Source/EMBER/AI/Base/BaseAI.cpp)

**관련 코드 1: [CAIController.cpp](Source/EMBER/AI/CAIController.cpp)**
```cpp
void ACAIController::OnPossess(APawn* InPawn)
{
	...
	Behavior = Cast<UCBehaviorTreeComponent>(AI->GetComponentByClass(UCBehaviorTreeComponent::StaticClass()));
	Behavior->SetBlackboard(Blackboard);
	RunBehaviorTree(AI->GetBehaviorTree());
}
```
설명: BaseAI에 섞여 있던 컨트롤러 책임(블랙보드/BT 실행)을 AIController로 분리해 역할을 명확히 정리.

**관련 코드 2: [CBehaviorTreeComponent.cpp](Source/EMBER/AI/BehaviorTree/CBehaviorTreeComponent.cpp)**
```cpp
void UCBehaviorTreeComponent::SetBlackboard_Object(FName Keyname, UObject* Value)
{
	Blackboard->SetValueAsObject(Keyname, Value);
}

void UCBehaviorTreeComponent::SetBlackboard_Vector(FName Keyname, FVector Value)
{
	Blackboard->SetValueAsVector(Keyname, Value);
}
```
설명: UCBehaviorTreeComponent를 만들어 BehaviorTree 블랙보드 키 값을 코드에서 일관되게 연동/제어.

**관련 코드 3: [CBTService_Aggressive.cpp](Source/EMBER/AI/Service/CBTService_Aggressive.cpp), [CBTService_Defensive.cpp](Source/EMBER/AI/Service/CBTService_Defensive.cpp)**
```cpp
// Aggressive / Defensive Service 분리 운용
```
설명: 기존에 Service 계층이 없어 세밀한 조정이 어려워, 공격 성향별 Service를 추가해 더 정밀한 전투 판단 튜닝이 가능하도록 구성.
## 플레이어 방어구 장착 시스템 구현
### ✔ 설계 의도
- ArmorComponent를 통해 방어구 장착 기능을 분리하고, 멀티플레이 환경에서 장착 상태가 동일하게 보이도록 동기화하는 것을 목표로 구현

### ✔ 구현 내용
#### ↳ [Armor/Equipment 네트워크 동기화](Source/EMBER/Component/ArmorComponent.cpp)

**관련 코드 1: [ArmorComponent.cpp](Source/EMBER/Component/ArmorComponent.cpp)**
```cpp
UArmorComponent::UArmorComponent()
{
	SetIsReplicatedByDefault(true);
}

void UArmorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UArmorComponent, ArmorDataArray);
}
```
설명: ArmorComponent를 직접 구현하고 리플리케이션을 설정해 방어구 데이터 동기화 기반을 구성.

**관련 코드 2: [ArmorComponent.cpp](Source/EMBER/Component/ArmorComponent.cpp)**
```cpp
void UArmorComponent::OnRep_ArmorDataArray()
{
	UpdateArmorVisuals();
}
```
설명: 장착 데이터가 복제되면 클라이언트에서 외형이 즉시 반영되도록 처리.

**관련 코드 3: [ArmorComponent.cpp](Source/EMBER/Component/ArmorComponent.cpp)**
```cpp
void UArmorComponent::EquipORUnEquip_Implementation(USkeletalMesh* Mesh, EArmorType ArmorType, int32 ItemTemplateID)
{
	if (HasOwnerAuthority())
	{
		...
		AddOrUpdateArmorData(ArmorType, ItemTemplateID, Mesh->GetPathName());
	}
}
```
설명: 서버 권한 기준으로 장착/해제 상태를 갱신하고, 복제 데이터에 반영해 네트워크 동기화를 보장.

## Main UI 구성 및 연동
### ✔ 설계 의도
- 구매한 Main UI 에셋을 프로젝트 진입 흐름에 결합해 메인 메뉴 -> 게임 시작 동선을 구성

### ✔ 구현 내용
#### ↳ [Main UI 에셋 적용 및 플로우 연결](Config/DefaultEngine.ini)
- 프로젝트 기본 맵을 MainMenu 레벨로 설정해 실행 즉시 UI 진입
- GameMode/GameInstance 연동으로 시작 흐름 일관화

```ini
[/Script/EngineSettings.GameMapsSettings]
EditorStartupMap=/Game/ProMainMenuV3/Levels/LVL_MainMenu_Test.LVL_MainMenu_Test
GameDefaultMap=/Game/ProMainMenuV3/Levels/LVL_MainMenu_Test.LVL_MainMenu_Test
GameInstanceClass=/Script/EMBER.EmberGameInstance
GlobalDefaultGameMode=/Game/GameMode/BP_EmberGameMode.BP_EmberGameMode_C
```

- 관련 에셋: [WB_MainMenu.uasset](Content/ProMainMenuV3/Widgets/WB_MainMenu.uasset)

# **Troubleshooting**

### 1) 🎯 AI 공격 중 멈춤 현상
- 문제: Dragon AI가 공격 후 Behavior Tree의 다음 행동을 수행하지 않고 멈춤
- 원인: DragonAttack Task가 즉시 `Succeeded`를 반환해 몽타주 기반 상태 복귀 타이밍과 불일치
- 해결: `ExecuteTask`는 `InProgress`를 반환하고 `OnMontageEnded`에서 `FinishLatentTask`로 종료 시점을 통일
- 관련 코드: [BTT_DragonAttack.cpp](Source/EMBER/AI/Task/BTT_DragonAttack.cpp)

```cpp
EBTNodeResult::Type UBTT_DragonAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	...
	return EBTNodeResult::InProgress;
}

void UBTT_DragonAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!BTComp || bInterrupted)
	{
		FinishLatentTask(*BTComp, EBTNodeResult::Failed);
		return;
	}

	FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}
```

### 2) 🎯 AI Sound가 맵 전체에서 들리는 문제
- 문제: AI 사운드가 플레이어 거리와 무관하게 맵 전체에서 동일하게 들려 몰입감을 해침
- 원인: 위치 기반 감쇠(Attenuation) 없이 사운드가 재생되어 청취 반경/볼륨 감쇠가 적용되지 않음
- 해결: AI Sound System을 도입해 `SoundType` 기반 분기 + `SoundAttenuation` 적용으로 일정 거리 내에서만 들리고, 멀어질수록 볼륨이 감쇠되도록 개선

```cpp
void ABaseAI::PlaySound(AISoundCategory InSoundType)
{
	if (SoundAttenuation == nullptr)
		return;
	if (AISounds[(int32)InSoundType] == nullptr)
		return;

	UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(),
		AISounds[(int32)InSoundType],
		GetActorLocation(),
		FRotator::ZeroRotator,
		1.0f, 1.0f, 0.0f,
		SoundAttenuation
	);
}
```
# **Retrospective (느낀점)**
- AI 리팩토링 과정에서 "기능 추가 속도"보다 "책임 분리"가 팀 개발에서 더 큰 생산성을 만든다는 점을 체감함
- “멀티 출시까지 이어지진 못했지만, 권한 체크/복제/RPC 경로를 미리 설계해 단일 기능도 확장 가능한 형태로 만들었고, ‘지금 필요한 구현’과 ‘다음 단계 확장성’을 함께 고려하는 개발 관점을 갖게 됨
- “AI 전투/사운드/데미지 이슈를 해결하면서, 버그의 원인은 기능 자체보다 시스템 간 경계(몽타주-BT-상태)에서 자주 발생한다는 것을 체감했고,  문제 재현-원인 분리-검증 코드 추가 순서로 디버깅 프로세스를 정착시키려 노력하게됨