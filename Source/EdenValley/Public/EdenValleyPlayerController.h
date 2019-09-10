// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EdenValleyInventoryInterface.h"
#include "Items/EItemType.h"
#include "Items/EdenValleyItemData.h"
#include "Interaction/Interactable.h"
#include "EdenValleyPlayerController.generated.h"

/** Base class for PlayerController. */
UCLASS()
class EDENVALLEY_API AEdenValleyPlayerController : public APlayerController, public IEdenValleyInventoryInterface
{
	GENERATED_BODY()

public:
	AEdenValleyPlayerController() {}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AInteractable* CurrentInteractable;

	/** Current money of the player. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Money;

	/** Map of all items owned by this player. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<int32, UEdenValleyItemData*> PlayerInventory;

	/** Array of all items of the potion inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UEdenValleyItemData*> PotionInventory;

	/** Array of all items of the potion inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UEdenValleyItemData*> SkillInventory;

	/** Array of all items of the potion inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UEdenValleyItemData*> WeaponInventory;

	/** Array of all items of the other inventory. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UEdenValleyItemData*> OtherInventory;

	/** Called when current money changes. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Inventory)
	FOnMoneyUpdated OnMoneyUpdated;

	/** Called when an inventory slot changed. */
	UPROPERTY()
	FOnSlottedItemChanged OnSlottedItemChanged;

	/** Called when the entire inventory loads. */
	UPROPERTY()
	FOnInventoryLoaded OnInventoryLoaded;

	/** Saves the inventorys. */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventorys();

	/** Loads the inventorys. */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventorys();

	/** Implement IEdenValleyInventoryInterface. */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual const int32 GetMoney() const override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void SetMoney(int32 MoneyToAdd) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool AddItemToInventoryByID(EItemType ItemType, FName ID, int32 ItemCount) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool AddItemToInventoryByItemData(UEdenValleyItemData* ItemData, int32 ItemCount) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool RemoveInventoryItem(UEdenValleyItemData* ItemData) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual int32 GetInventoryItemCount(UEdenValleyItemData* ItemData) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual int32 SetInventoryItemCount(UEdenValleyItemData* ItemData, int32 ItemCount) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual UEdenValleyItemData* GetSlottedItemData(int32 SlotNumber) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool SetSlottedItem(int32 SlotNumber, UEdenValleyItemData* ItemData) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void GetInventoryArray(TArray<UEdenValleyItemData*>& ItemData, EItemType ItemType) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void GetInventoryArrayOfPurchasableItems(TArray< UEdenValleyItemData*>& PurchasableInventory) override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual const TMap<int32, UEdenValleyItemData*>& GetPlayerInventory() const override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual const TArray<UEdenValleyItemData*>& GetPotionInventory() const override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual const TArray<UEdenValleyItemData*>& GetSkillInventory() const override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual const TArray<UEdenValleyItemData*>& GetWeaponInventory() const override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual const TArray<UEdenValleyItemData*>& GetOtherInventory() const override;

	/** Called when an item has suffered a change. */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(bool bAdded, UEdenValleyItemData* ItemData);

	/** Called when a slotted item has suffered a change. */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventorySlottedItemChanged(int32 SlotNumber, UEdenValleyItemData* ItemData);

	/** Returns OnMoneyUpdated delegate. */
	virtual FOnMoneyUpdated& GetMoneyUpdatedDelegate() override
	{
		return OnMoneyUpdated;
	}

	/** Returns OnSlottedItemChanged delegate. */
	virtual FOnSlottedItemChanged& GetSlottedItemChangedDelegate() override
	{
		return OnSlottedItemChanged;
	}

	/** Returns OnInventoryLoaded delegate. */
	virtual FOnInventoryLoaded& GetInventoryLoadedDelegate() override
	{
		return OnInventoryLoaded;
	}

protected:
	/** Calls the inventory update callbacks. */
	void NotifyInventoryLoaded();
	void NotifyInventoryItemChanged(bool bAdded, UEdenValleyItemData* ItemData);
	void NotifySlottedItemChanged(int32 SlotNumber, UEdenValleyItemData* ItemData);

private:
	/** Intermediate methods to run a specific task. */
	bool AddItemToInventory(TArray<UEdenValleyItemData*>& Inventory, UDataTable * DataTable, FName ID, int32 ItemCount);
	bool RemoveItemFromInventory(TArray<UEdenValleyItemData*>& Inventory, UEdenValleyItemData* ItemData);
	UEdenValleyItemData* FindItemByID(TArray<UEdenValleyItemData*> Inventory, FName ID);
	void AddPurchasableItemsToInventory(TArray<UEdenValleyItemData*>& PurchasableInventory, EItemType ItemType);
};