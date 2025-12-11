#include "AI/BehaviorTree/DragonBehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// void UDragonBehaviorTreeComponent::SetAttackState(EDragonAttackState InState)
// {
// 	if (Blackboard == nullptr) return;
//
// 	Blackboard->SetValueAsEnum(AttackStateKey, static_cast<uint8>(InState));
// }
//
// EDragonAttackState UDragonBehaviorTreeComponent::GetAttackState() const
// {
// 	if (Blackboard == nullptr) return EDragonAttackState::None;
//
// 	return static_cast<EDragonAttackState>(Blackboard->GetValueAsEnum(AttackStateKey));
// }
//
// bool UDragonBehaviorTreeComponent::IsNormalAttack() const
// {
// 	return GetAttackState() == EDragonAttackState::Normal;
// }
//
// bool UDragonBehaviorTreeComponent::IsComboAttack() const
// {
// 	return GetAttackState() == EDragonAttackState::Combo;
// }
//
// bool UDragonBehaviorTreeComponent::IsSpitAttack() const
// {
// 	return GetAttackState() == EDragonAttackState::Spit;
// }
//
// bool UDragonBehaviorTreeComponent::IsBreathAttack() const
// {
// 	return GetAttackState() == EDragonAttackState::Breath;
// }
//
// void UDragonBehaviorTreeComponent::SetNormalAttackMode()
// {
// 	SetActionMode();
// 	SetAttackState(EDragonAttackState::Normal);
// }
//
// void UDragonBehaviorTreeComponent::SetComboAttackMode()
// {
// 	SetActionMode();
// 	SetAttackState(EDragonAttackState::Combo);
// }
//
// void UDragonBehaviorTreeComponent::SetSpitAttackMode()
// {
// 	SetActionMode();
// 	SetAttackState(EDragonAttackState::Spit);
// }
//
// void UDragonBehaviorTreeComponent::SetBreathAttackMode()
// {
// 	SetActionMode();
// 	SetAttackState(EDragonAttackState::Breath);
// }
//
// void UDragonBehaviorTreeComponent::SetBlackboard_Enum(FName Keyname, EDragonAttackState Value)
// {
// 	if (Blackboard == nullptr) return;
// 	
// 	Blackboard->SetValueAsEnum(Keyname, static_cast<uint8>(Value));
// }
