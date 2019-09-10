// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/FEdenValleyItem.h"
#include "FEdenValleySkillItem.generated.h"

/** Native base class for skills. */
USTRUCT(BlueprintType)
struct FEdenValleySkillItem : public FEdenValleyItem
{
	GENERATED_BODY()

public:
	/** Constructor. */
	FEdenValleySkillItem()
	{
		ItemType = EItemType::IT_Skill;
	}
};