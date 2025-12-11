#include "AI/Base/HumanAIBase.h"

#include "C_CharacterMovementComponent.h"
#include "C_StateComponent.h"
#include "MontageSystemComponent.h"
#include "StatusComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/CBehaviorTreeComponent.h"

AHumanAIBase::AHumanAIBase()
{
	//MoveComponent = CreateDefaultSubobject<UC_CharacterMovementComponent>(TEXT("Move Component"));
	/*Behavior = CreateDefaultSubobject<UCBehaviorTreeComponent>(TEXT("BehaviorTree"));*/
}

void AHumanAIBase::BeginPlay()
{
	Super::BeginPlay();
	AIMoveComponent->OnWalk();
	//StateComponent->OnStateTypeChanged.AddDynamic(this, &AHumanAIBase::OnStateTypeChanged);

}

//void AHumanAIBase::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
//{
//}

