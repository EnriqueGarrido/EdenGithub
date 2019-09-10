// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/FEdenValleyItem.h"
#include "FEdenValleyPotionItem.generated.h"

/** Native base class for potions. */
USTRUCT(BlueprintType)
struct FEdenValleyPotionItem : public FEdenValleyItem
{
	GENERATED_BODY()

public:
	/** Constructor. */
	FEdenValleyPotionItem()
	{
		ItemType = EItemType::IT_Potion;
	}
};