// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Styling/SlateBrush.h"
#include "GameplayAbility.h"
#include "Items/EItemType.h"
#include "FEdenValleyItem.generated.h"

/** Base class for all items. */
USTRUCT(BlueprintType)
struct FEdenValleyItem : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** Constructor. */
	FEdenValleyItem()
		: ItemType(EItemType::IT_None) 
		, ItemID("")
		, Name(FText())
		, ItemPrice(0)
		, Description(FText())
		, bCanBeEquipped(false)
		, bIsConsumable(false)
		, ItemSlot(-1)
		, AbilityLevel(1)
		, ItemCount(0)
	{}

	/** Type of the item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EItemType ItemType;

	/** Identifier of the item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName ItemID;

	/** Name of the item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FText Name;

	/** Price in game. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemPrice;

	/** Description of the item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FText Description;

	/** Icon of the item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FSlateBrush ItemIcon;

	/** Allows this item to be equipped if true. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bCanBeEquipped;

	/** Allows this item to be removed every time is used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	bool bIsConsumable;

	/** Slot that represents the position that this item must fill. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 ItemSlot;

	/** Ability to grant if this item is slotted. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<UGameplayAbility> GrantedAbility;

	/** Ability level this item grants. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	int32 AbilityLevel;

	/** Number of instances of this item. We don't use EditAnywhere because this attribute is modified at run-time. */
	UPROPERTY(BlueprintReadWrite)
	int32 ItemCount;

	bool operator==(const struct FEdenValleyItem& OtherItem) const
	{
		if (ItemID == OtherItem.ItemID) return true;
		return false;
	}
};
