// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/FEdenValleyItem.h"
#include "EdenValleyItemData.generated.h"

/** Wrapper class to maintain a reference of items. */
UCLASS(BlueprintType)
class EDENVALLEY_API UEdenValleyItemData : public UObject
{
	GENERATED_BODY()
	
public:
	/** Item structure. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FEdenValleyItem Item;
};
