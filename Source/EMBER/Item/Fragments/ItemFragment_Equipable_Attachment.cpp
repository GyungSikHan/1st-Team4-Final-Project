// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFragment_Equipable_Attachment.h"

UItemFragment_Equipable_Attachment::UItemFragment_Equipable_Attachment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    EquipmentInfo.PlayRate = 1.0f;
}

FEquipment UItemFragment_Equipable_Attachment::GetEquipmentInfo() const
{
    return EquipmentInfo;
}
