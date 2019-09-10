// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Items/EItemType.h"
#include "../Items/FEdenValleyItem.h"
#include "FEdenValleyInventoryData.h"
#include "EdenValleySaveGame.generated.h"

/** Object that is written to and read from the save game archive. */
UCLASS(BlueprintType)
class EDENVALLEY_API UEdenValleySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/** All inventory items data. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FEdenValleyInventoryData InventoryData;

	/** User's unique id */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FString UserId;
};