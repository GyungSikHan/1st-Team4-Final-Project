#include "AI/Task/BTT_ResetValue.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTT_ResetValue::UBTT_ResetValue()
{
	NodeName = TEXT("ResetValue");
}

EBTNodeResult::Type UBTT_ResetValue::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = Comp.GetBlackboardComponent();
	if (!BB || !BlackboardKey.SelectedKeyType)
	{
		return EBTNodeResult::Failed;
	}

	const FName KeyName = BlackboardKey.SelectedKeyName;
	const UClass* KeyClass = BlackboardKey.SelectedKeyType;

	if (KeyClass == UBlackboardKeyType_Bool::StaticClass())
	{
		BB->SetValueAsBool(KeyName, false);
	}

	else if (KeyClass == UBlackboardKeyType_Int::StaticClass())
	{
		BB->SetValueAsInt(KeyName, 0);
	}

	else if (KeyClass == UBlackboardKeyType_Vector::StaticClass())
	{
		BB->SetValueAsVector(KeyName, FVector::ZeroVector);
	}

	else if (KeyClass == UBlackboardKeyType_Object::StaticClass())
	{
		BB->ClearValue(KeyName);
	}

	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
