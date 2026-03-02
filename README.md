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
- **SYMBIO**: Unreal Engine 5.5 기반 4인 팀 Survival Action 프로젝트로,
- **핵심 구현**
	
- **가장 어려웠던 점 → 해결**
  
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

"**EMBER: The Eternal Blizzard**"는 언리얼 엔진 기반으로 제작된 Survival Action 게임으로, 얼어붙은 세상을 구하기 위해 몬스터를 물리치고 각종 몬스터를 잡으며 영혼의 불꽃을 찾아 모험을 떠나는 세계관을 담고 있습니다.
플레이어는 추운 환경에서 살아남아 영혼의 불꽃으로 얼어붙은 세상을 구해야 합니다.

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
### ✔ 구현 내용
#### ↳ [AI Weapon](https://github.com/GyungSikHan/1st-Team4-Final-Project/blob/Dev/Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp)
##### ✔ 설계 의도
  - AI의 공격 애니메이션 재생시 AI의 충돌체에 충돌하는 것이 아닌 좀더 정밀한 충돌을 위해 충돌체를 따로 만들어 충돌을 감지할때만 데미지를 전달하도록 구현  


#### [충돌체 생성](https://github.com/GyungSikHan/1st-Team4-Final-Project/blob/Dev/Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp#L18-L55) 
  - 여러개의 충돌체를 만들고, 공격을 하기 전 충돌을 하지 못하도록 예외처리를 하여 공격시에만 충돌할 수 있도록 함
```c++
void ACAI_Weapon::SpawnPlay_Implementation(ACharacter* InOwner)
{
	...

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);
		USphereComponent* sphere = Cast<USphereComponent>(child);
		if(shape != nullptr)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this,&ACAI_Weapon::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAI_Weapon::OnComponentEndOverlap);
			Collisions.Add(child);
		}
		else if(sphere != nullptr)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAI_Weapon::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAI_Weapon::OnComponentEndOverlap);
			Collisions.Add(child);
		}
	}
	OffCollision();
}
```
#### [충돌체 부착](https://github.com/GyungSikHan/1st-Team4-Final-Project/blob/Dev/Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp#L98-L107)
  - AI의 Skeleton에 심어둔 소켓의 이름과 만든 충돌체의 이름이 같을 때 해당 소켓에 충돌체를 붙임
```c++
void ACAI_Weapon::AttachTo(FName InSocketName)
{
	if(OwnerCharacter == nullptr)
		return;
	for (USceneComponent* collision : Collisions)
	{
		if(collision->GetName() == InSocketName.ToString())
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
	}
}
```

#### [충돌 처리](https://github.com/GyungSikHan/1st-Team4-Final-Project/blob/Dev/Source/EMBER/AI/AIWeapon/CAI_Weapon.cpp#L77-L96)
- 공격을 하는 몽타주가 재생시 Collision을 키고 끌 수 있게 구현
```c++
void ACAI_Weapon::OnCollision()
{
	// UE_LOG(LogTemp, Warning, L"collision");
	for (USceneComponent* collision : Collisions)
	{
		UPrimitiveComponent* coll = Cast<UPrimitiveComponent >(collision);
		if (coll != nullptr)
			coll->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAI_Weapon::OffCollision()
{
	for (USceneComponent* collision : Collisions)
	{
		UPrimitiveComponent* coll = Cast<UPrimitiveComponent >(collision);
		if (coll != nullptr)
			coll->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
```

#### ↳ [AI Combat System]()

```c++

```


#### ↳ [AI Damage System]()
#### ↳ [AI 구조 개선]()
#### ↳ [AI Sound System]()

## 방어구 장착 시스템 구현
### ✔ 설계 의도
- Component 기반 장비 장착 구조로 설계하여 기능을 재사용하기 쉽고, 분리, 조합하여 Actor를 가볍고 유연하게 만들기 위해 사용
- Network 동기화 처리를 통해 멀티 환경에서도 동작할 수 있도록 구현

### ✔ 구현 내용
#### ↳ []()

## Main UI 구성 및 연동
### ✔ 설계 의도
![alt text](image.png)
- 구매한 에셋을 프로젝트에 적용시켜 Level Load 및 게임 진입 흐름 연결
### ✔ 구현 내용
#### ↳ []()

# **Troubleshooting**

### 1) 🎯 AI 공격 중 멈춤 현상
- 문제: Dragon AI가 공격을 하고나서 Behavior Tree의 다음 행동을 하지 않는 버그 발생
- 원인: DragonAttack의 ExecuteTask 함수에서 EBTNodeResult::Succeeded를 return하게 되면서 Montage안에 있던 AI Attack state를 Attack에서 Idle 상태로 돌아가지 못함
- 해결: ExecuteTask 함수의 EBTNodeResult::InProgress를 리턴하고 OnMontageEnded 함수를 추가하여 몽타주가 끝나면 FinishLatentTask에 EBTNodeResult::Succeeded로 Task를 끝날 수 있게 하여 State를 Attack에서 Idle로 변경 가능하게 끔 구현
[UBTT_DragonAttack](https://github.com/GyungSikHan/1st-Team4-Final-Project/blob/Dev/Source/EMBER/AI/Task/BTT_DragonAttack.cpp#L16-L73)
```cpp
UBTT_DragonAttack::UBTT_DragonAttack()
{
	NodeName = TEXT("DragonRangedAttack");
	bNotifyTick = false;
}

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

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("FinishDragonAttackMontage")));
	FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}
```

### 2) 🎯 프로젝트 실행 시 프로젝트가 멈추는 문제 발생
- 문제: 테스트를 위해 프로젝트 실행을 하면 프로젝트가 멈추고 종료도 안되는 문제 발생
- 원인: Level에 너무 많은 Object들이 배치되어 리소스가 과하게 사용되는 문제 때문에 멈춤
- 해결1: 환경 요소를 맵 전체가 아닌 플레이어가 갈 수 있는 위치에만 배치하여 효율적으로 맵을 구성
- 해결2: AI가 과하게 배치되어 매 Tick마다 실행되는 Behavior Tree가 계속해서 리소르를 너무 많이 잡아먹어 Player 주변에 생성된 AI들만 Behavior Tree가 실행되도록 지시함

# **Retrospective (느낀점)**
- AI를 리팩토링하는 과정에서 다른 사람의 코드를 읽고 파악하는 것의 중요성을 느끼게 되었음
- 그렇지 않은 경우 같은 기능(예를 들면 AI가 공격을 하는 Task 개발)을 상위 클래스를 상속을 받아 만들거나 이미 만들어진 기능을 사용하는 것이 아닌 중복되는 기능을 만들게 되었으며, 문제가 생겼을 때 마다 어느 코드가 문제인지 파악하는데 어려움을 느끼게 되었음
- 