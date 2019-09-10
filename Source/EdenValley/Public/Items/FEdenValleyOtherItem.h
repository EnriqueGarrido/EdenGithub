// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/FEdenValleyItem.h"
#include "FEdenValleyOtherItem.generated.h"

/** Native base class for any item different from potion, weapon or skill. */
USTRUCT(BlueprintType)
struct FEdenValleyOtherItem : public FEdenValleyItem
{
	GENERATED_BODY()

public:
	/** Constructor. */
	FEdenValleyOtherItem()
	{
		ItemType = EItemType::IT_Other;
	}
};