// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_None				UMETA(DisplayName = "None"),
	IT_Potion			UMETA(DisplayName = "Potion"),
	IT_Weapon			UMETA(DisplayName = "Weapon"),
	IT_Skill			UMETA(DisplayName = "Skill"),
	IT_Other			UMETA(DisplayName = "Other")
};