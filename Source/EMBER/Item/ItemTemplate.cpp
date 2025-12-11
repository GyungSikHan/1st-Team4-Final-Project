// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTemplate.h"

UItemTemplate::UItemTemplate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UItemFragment* UItemTemplate::FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (UItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
