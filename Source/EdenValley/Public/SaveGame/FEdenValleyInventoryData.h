// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Items/EItemType.h"
#include "../Items/FEdenValleyItem.h"
#include "FEdenValleyInventoryData.generated.h"

/** Struct representing the inventory data. */
USTRUCT(BlueprintType)
struct FEdenValleyInventoryData
{
	GENERATED_BODY()

public:
	/** Current money of the player. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InventoryData)
	int32 Money;

	/** Map of all items owned by this player. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InventoryData)
	TMap<int32, FEdenValleyItem> PlayerInventory;

	/** Array of all items of the potion inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InventoryData)
	TArray<FEdenValleyItem> PotionInventory;

	/** Array of all items of the potion inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InventoryData)
	TArray<FEdenValleyItem> SkillInventory;

	/** Array of all items of the potion inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InventoryData)
	TArray<FEdenValleyItem> WeaponInventory;

	/** Array of all items of the other inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InventoryData)
	TArray<FEdenValleyItem> OtherInventory;
};