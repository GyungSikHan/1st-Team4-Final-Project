// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_Random.h"

#include "Kismet/KismetMathLibrary.h"

UBTDecorator_Random::UBTDecorator_Random()
{
	NodeName = "Random";
}

bool UBTDecorator_Random::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	int32 random = UKismetMathLibrary::RandomInteger(5);

	return random != 0;
}
