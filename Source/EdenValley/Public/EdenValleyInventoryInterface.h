// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Items/EItemType.h"
#include "Items/EdenValleyItemData.h"
#include "EdenValleyInventoryInterface.generated.h"

/** Delegates called when the current money changes. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyUpdated, int32, MoneyToAdd);

/** Delegate called when the contents of an inventory slot change. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged, int32, SlotNumber, UEdenValleyItemData*, ItemData);

/** Delegate called when the entire inventory has been loaded. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryLoaded);

/** Inventory interface to be inherited by PlayerController */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEdenValleyInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class EDENVALLEY_API IEdenValleyInventoryInterface
{
	GENERATED_BODY()

public:
	/** Returns current money. */
	virtual const int32 GetMoney() const = 0;

	/** Sets current money. */
	virtual void SetMoney(int32 MoneyToAdd) = 0;

	/** Returns the map of items that player owns. */
	virtual const TMap<int32, UEdenValleyItemData*>& GetPlayerInventory() const = 0;

	/** Returns the inventory Array with potions. */
	virtual const TArray<UEdenValleyItemData*>& GetPotionInventory() const = 0;

	/** Returns the inventory Array with skills. */
	virtual const TArray<UEdenValleyItemData*>& GetSkillInventory() const = 0;

	/** Returns the inventory Array with weapons. */
	virtual const TArray<UEdenValleyItemData*>& GetWeaponInventory() const = 0;

	/** Returns the inventory Array with items of different types. */
	virtual const TArray<UEdenValleyItemData*>& GetOtherInventory() const = 0;

	/** Returns inventory Array based on a type. */
	virtual void GetInventoryArray(TArray<UEdenValleyItemData*>& ItemData, EItemType ItemType) = 0;

	/** Returns an array of items that can be bought. */
	virtual void GetInventoryArrayOfPurchasableItems(TArray< UEdenValleyItemData*>& PurchasableInventory) = 0;

	/** Returns if the item given by an ID has been added correctly. */
	virtual bool AddItemToInventoryByID(EItemType ItemType, FName ID, int32 ItemCount) = 0;

	/** Returns if the given item has been added correctly. */
	virtual bool AddItemToInventoryByItemData(UEdenValleyItemData* ItemData, int32 ItemCount) = 0;

	/** Returns true if the items is removed correctly. */
	virtual bool RemoveInventoryItem(UEdenValleyItemData* ItemData) = 0;

	/** Returns the number of instances of the item passed as a parameter. */
	virtual int32 GetInventoryItemCount(UEdenValleyItemData* ItemData) = 0;

	/** Returns true if the item count is modified correctly. */
	virtual int32 SetInventoryItemCount(UEdenValleyItemData* ItemData, int32 ItemCount) = 0;

	/** Returns the item based on its slot number. */
	virtual UEdenValleyItemData* GetSlottedItemData(int32 SlotNumber) = 0;

	/** Returns true if the item is equipped correctly. */
	virtual bool SetSlottedItem(int32 SlotNumber, UEdenValleyItemData* ItemData) = 0;

	/** Returns the delegate for when money changes. */
	virtual FOnMoneyUpdated& GetMoneyUpdatedDelegate() = 0;

	/** Returns the delegate for inventory slot changes. */
	virtual FOnSlottedItemChanged& GetSlottedItemChangedDelegate() = 0;

	/** Returns the delegate for when the inventory loads. */
	virtual FOnInventoryLoaded& GetInventoryLoadedDelegate() = 0;
};
