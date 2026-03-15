# **EMBER: The Eternal Blizzard**
<table>
	<tr>
		<td align="center">
			<img src="Image/image3.png" width="1000"><br>
		</td>
	</tr>
</table>

# ⚡ 30초 요약 (TL;DR)
- **EMBER: The Eternal Blizzard**: Unreal Engine 5.5 기반 6인 팀 Survival Action 프로젝트
- **내 핵심 구현**
  - AI 전투 로직(Combat/Weapon/Damage/Sound) 설계 및 구현
  - Main UI 에셋 연동(메인 메뉴 -> 게임 진입 흐름 연결)
  - 플레이어 방어구 장착 시스템 구현(네트워크 동기화)
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
- AI 전투 기능을 `Combat / Weapon / Damage / Sound`로 분리해 재사용성과 유지보수성을 높임
- 몽타주, BT Task, Notify 책임을 분리해 상태 전이를 안정화

### ✔ 구현 내용
#### ↳ [AI Combat System](Source/EMBER/AI/Task/BTT_DragonAttack.cpp)
- 몽타주 기반 원거리 공격을 BT Task에서 `InProgress`로 유지하고, 종료 콜백에서 태스크를 종료해 전투 흐름을 안정화

```cpp
EBTNodeResult::Type UBTT_DragonAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	...
	DragonAnim->Montage_SetEndDelegate(EndDelegate, RangedAttackMontage);
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

#### ↳ [AI Weapon](Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp)
- 전용 충돌체를 동적으로 수집하고, 공격 구간에만 충돌을 활성화해 잘못 감지된 타격을 줄임

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

void ACAI_Weapon::OnCollision()
{
	for (USceneComponent* collision : Collisions)
	{
		if (UPrimitiveComponent* coll = Cast<UPrimitiveComponent>(collision))
			coll->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
```

#### ↳ [AI Damage System](Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp)
- 중복 히트 방지(`Hitted`)를 적용하고, 확정 타격 시점에만 데미지를 전달
- **GameData 기반 데미지 파라미터를 설계/구현**해 AI 공격별 데미지 값을 데이터로 관리하고, 코드 수정 없이 밸런싱 가능하게 구성

```cpp
if (OwnerCharacter == other)
	return;
if (OwnerCharacter->GetClass() == other->GetClass())
	return;

for (ACharacter* hitted : Hitted)
	if (hitted == other)
		return;

Hitted.AddUnique(other);
HitDatas[CurrAttackIndex].SendDamage(OwnerCharacter, this, other);
```

#### ↳ [AI Sound System](Source/EMBER/AI/Notify/CAnimNotify_AISound.cpp)
- Anim Notify에서 상황별 사운드 타입을 전달해 AI별 사운드 연출을 일관화

```cpp
void UCAnimNotify_AISound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	...
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(MeshComp->GetOwner());
	if (ai == nullptr)
		return;
	ai->PlaySound(SoundType);
}
```

#### ↳ [AI 구조 개선](Source/EMBER/AI/Base/BaseAI.cpp)
- AI 베이스/BT 컴포넌트 중심으로 공통 로직을 묶어 패턴별 AI 확장 시 중복 구현을 줄임
- 관련 코드: [BaseAI.cpp](Source/EMBER/AI/Base/BaseAI.cpp), [CBehaviorTreeComponent.cpp](Source/EMBER/AI/BehaviorTree/CBehaviorTreeComponent.cpp)

## 플레이어 방어구 장착 시스템 구현
### ✔ 설계 의도
- Component 기반 장착 구조로 기능 분리를 유지하고 네트워크 동기화를 보장

### ✔ 구현 내용
#### ↳ [Armor/Equipment 네트워크 동기화](Source/EMBER/Component/ArmorComponent.cpp)
- 서버 권한으로 장비 상태를 갱신하고, `Replicated Array + OnRep`로 클라이언트 외형을 동기화

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

void UArmorComponent::OnRep_ArmorDataArray()
{
	UpdateArmorVisuals();
}
```

- 인벤토리 장착 데이터는 FastArraySerializer로 복제해 슬롯 단위 변경 반영

```cpp
bool FEquipList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FEquipEntry, FEquipList>(Entries, DeltaParms,*this);
}
```

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

### 2) 🎯 프로젝트 실행 시 프리징 문제
- 문제: 테스트 실행 시 프로젝트가 멈추고 종료되지 않는 현상 발생
- 원인: 레벨 내 오브젝트/AI 배치 밀도가 높아 Tick/BT 부하가 집중됨
- 해결: 환경 오브젝트 배치를 플레이 가능 영역 중심으로 재구성하고, AI 활성 범위를 플레이어 인접 구간 중심으로 제한

# **Retrospective (느낀점)**
- AI 리팩토링 과정에서 "기능 추가 속도"보다 "책임 분리"가 팀 개발에서 더 큰 생산성을 만든다는 점을 체감함
- BT Task와 Montage/Notify의 완료 시점을 분리해서 보면 버그가 길어지므로, 상태 전이 기준을 먼저 정의한 뒤 기능을 붙이는 습관을 갖게 됨
- 방어구 장착 시스템을 멀티플레이 기준으로 구현하며 서버 권한 흐름을 먼저 설계하는 방식의 중요성을 학습함





