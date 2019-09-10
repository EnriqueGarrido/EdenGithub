// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenValleyPlayerController.h"
#include "EdenValleyGameStateBase.h"
#include "EdenValleyGameInstance.h"
#include "SaveGame/EdenValleySaveGame.h"

bool AEdenValleyPlayerController::SaveInventorys()
{
	UWorld* World = GetWorld();
	UEdenValleyGameInstance* GameInstance = World ? World->GetGameInstance<UEdenValleyGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UEdenValleySaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		// Resets cached data in save game before writing to it.
		CurrentSaveGame->InventoryData.PlayerInventory.Reset();
		CurrentSaveGame->InventoryData.WeaponInventory.Reset();
		CurrentSaveGame->InventoryData.PotionInventory.Reset();
		CurrentSaveGame->InventoryData.SkillInventory.Reset();
		CurrentSaveGame->InventoryData.OtherInventory.Reset();
		CurrentSaveGame->InventoryData.Money = 0;

		for (const TPair<int32, UEdenValleyItemData*>& Pair : PlayerInventory)
		{
			if (Pair.Key)
			{
				CurrentSaveGame->InventoryData.PlayerInventory.Add(Pair.Key, Pair.Value->Item);
			}
		}

		for (const UEdenValleyItemData* ItemData : WeaponInventory)
		{
			CurrentSaveGame->InventoryData.WeaponInventory.Add(ItemData->Item);
		}

		for (const UEdenValleyItemData* ItemData : PotionInventory)
		{
			CurrentSaveGame->InventoryData.PotionInventory.Add(ItemData->Item);
		}

		for (const UEdenValleyItemData* ItemData : SkillInventory)
		{
			CurrentSaveGame->InventoryData.SkillInventory.Add(ItemData->Item);
		}

		for (const UEdenValleyItemData* ItemData : OtherInventory)
		{
			CurrentSaveGame->InventoryData.OtherInventory.Add(ItemData->Item);
		}
		CurrentSaveGame->InventoryData.Money = Money;

		// Now that cache is updated; we can write to disk.
		GameInstance->WriteSaveGame();
		return true;
	}

	return false;
}

bool AEdenValleyPlayerController::LoadInventorys()
{
	UWorld* World = GetWorld();
	UEdenValleyGameInstance* GameInstance = World ? World->GetGameInstance<UEdenValleyGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UEdenValleySaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		// Resets cached data in save game before writing to it.
		PlayerInventory.Reset();
		WeaponInventory.Reset();
		PotionInventory.Reset();
		SkillInventory.Reset();
		OtherInventory.Reset();
		Money = 0;

		for (const TPair<int32, FEdenValleyItem>& Pair : CurrentSaveGame->InventoryData.PlayerInventory)
		{
			if (Pair.Key)
			{
				UEdenValleyItemData* ItemData = NewObject<UEdenValleyItemData>(this);
				ItemData->Item = Pair.Value;
				PlayerInventory.Add(Pair.Key, ItemData);
				NotifySlottedItemChanged(Pair.Key, ItemData);
			}
		}

		for (const FEdenValleyItem& Item : CurrentSaveGame->InventoryData.WeaponInventory)
		{
			UEdenValleyItemData* ItemData = NewObject<UEdenValleyItemData>(this);
			ItemData->Item = Item;
			WeaponInventory.Add(ItemData);
			NotifyInventoryItemChanged(true, ItemData);
		}

		for (const FEdenValleyItem& Item : CurrentSaveGame->InventoryData.PotionInventory)
		{
			UEdenValleyItemData* ItemData = NewObject<UEdenValleyItemData>(this);
			ItemData->Item = Item;
			PotionInventory.Add(ItemData);
			NotifyInventoryItemChanged(true, ItemData);
		}

		for (const FEdenValleyItem& Item : CurrentSaveGame->InventoryData.SkillInventory)
		{
			UEdenValleyItemData* ItemData = NewObject<UEdenValleyItemData>(this);
			ItemData->Item = Item;
			SkillInventory.Add(ItemData);
			NotifyInventoryItemChanged(true, ItemData);
		}

		for (const FEdenValleyItem& Item : CurrentSaveGame->InventoryData.OtherInventory)
		{
			UEdenValleyItemData* ItemData = NewObject<UEdenValleyItemData>(this);
			ItemData->Item = Item;
			OtherInventory.Add(ItemData);
			NotifyInventoryItemChanged(true, ItemData);
		}
		SetMoney(CurrentSaveGame->InventoryData.Money);
		NotifyInventoryLoaded();
		return true;
	}

	return false;
}

const int32 AEdenValleyPlayerController::GetMoney() const
{
	return Money;
}

void AEdenValleyPlayerController::SetMoney(int32 MoneyToAdd)
{
	Money += MoneyToAdd;
	OnMoneyUpdated.Broadcast(MoneyToAdd);
}

bool AEdenValleyPlayerController::AddItemToInventoryByID(EItemType ItemType, FName ID, int32 ItemCount)
{
	bool bAdded = false;

	AEdenValleyGameStateBase* GameState = Cast<AEdenValleyGameStateBase>(GetWorld()->GetGameState());
	UDataTable* TableDB = GameState->GetTableDB(ItemType); // Get table based on the item type.
	switch (ItemType)
	{
	case EItemType::IT_Potion:
		bAdded = AddItemToInventory(PotionInventory, TableDB, ID, ItemCount);
		break;
	case EItemType::IT_Skill:
		bAdded = AddItemToInventory(SkillInventory, TableDB, ID, ItemCount);
		break;
	case EItemType::IT_Weapon:
		bAdded = AddItemToInventory(WeaponInventory, TableDB, ID, ItemCount);
		break;
	case EItemType::IT_Other:
		bAdded = AddItemToInventory(OtherInventory, TableDB, ID, ItemCount);
		break;
	}
	
	if (bAdded)
	{
		return true;
	}

	return false;
}

bool AEdenValleyPlayerController::AddItemToInventoryByItemData(UEdenValleyItemData * ItemData, int32 ItemCount)
{
	bool bAdded = false;

	EItemType ItemType = ItemData->Item.ItemType;
	AEdenValleyGameStateBase* GameState = Cast<AEdenValleyGameStateBase>(GetWorld()->GetGameState());
	UDataTable* TableDB = GameState->GetTableDB(ItemType); // Get table based on the item type.

	TArray<UEdenValleyItemData*> Inventory;
	GetInventoryArray(Inventory, ItemType);

	// Set item count whether it exists or not.
	ItemData->Item.ItemCount += ItemCount;

	// If Item already exists, we do nothing.
	if (FindItemByID(Inventory, ItemData->Item.ItemID))
	{
		return true;
	}

	switch (ItemType)
	{
	case EItemType::IT_Potion:
		PotionInventory.Add(ItemData);
		bAdded = true;
		break;
	case EItemType::IT_Skill:
		SkillInventory.Add(ItemData);
		bAdded = true;
		break;
	case EItemType::IT_Weapon:
		WeaponInventory.Add(ItemData);
		bAdded = true;
		break;
	case EItemType::IT_Other:
		OtherInventory.Add(ItemData);
		bAdded = true;
		break;
	}

	if (bAdded)
	{
		NotifyInventoryItemChanged(bAdded, ItemData);
		return true;
	}

	return false;
}

bool AEdenValleyPlayerController::RemoveInventoryItem(UEdenValleyItemData* ItemData)
{
	bool bRemoved = false;

	EItemType ItemType = ItemData->Item.ItemType;
	switch (ItemType)
	{
	case EItemType::IT_Potion:
		bRemoved = RemoveItemFromInventory(PotionInventory, ItemData);
		break;
	case EItemType::IT_Skill:
		bRemoved = RemoveItemFromInventory(SkillInventory, ItemData);
		break;
	case EItemType::IT_Weapon:
		bRemoved = RemoveItemFromInventory(WeaponInventory, ItemData);
		break;
	case EItemType::IT_Other:
		bRemoved = RemoveItemFromInventory(OtherInventory, ItemData);
		break;
	}

	if (bRemoved) 
	{
		return true;
	}

	return false;
}

int32 AEdenValleyPlayerController::GetInventoryItemCount(UEdenValleyItemData* ItemData)
{
	EItemType ItemType = ItemData->Item.ItemType;
	TArray<UEdenValleyItemData*> Inventory;
	GetInventoryArray(Inventory, ItemType);

	// We get the instances of an item in the inventory because it is possible that we want to look for
	// items that are not already added in it. For example, the purchasable item instances.
	UEdenValleyItemData* ItemDataFound = FindItemByID(Inventory, ItemData->Item.ItemID);
	if (ItemDataFound)
	{
		return ItemDataFound->Item.ItemCount;
	}
	return 0;
}

int32 AEdenValleyPlayerController::SetInventoryItemCount(UEdenValleyItemData* ItemData, int32 ItemCount)
{
	int32 NewItemCount = ItemData->Item.ItemCount += ItemCount;
	return NewItemCount;
}

UEdenValleyItemData* AEdenValleyPlayerController::GetSlottedItemData(int32 SlotNumber)
{
	if (PlayerInventory.Contains(SlotNumber))
	{
		UEdenValleyItemData* ItemData = PlayerInventory[SlotNumber];
		return ItemData;
	}
	return nullptr;
}

bool AEdenValleyPlayerController::SetSlottedItem(int32 SlotNumber, UEdenValleyItemData* ItemData)
{
	bool bSetSlot = false;

	if (ItemData)
	{
		if (!PlayerInventory.Contains(SlotNumber))
		{
			// Add a new slot.
			PlayerInventory.Add(SlotNumber, ItemData);
			NotifySlottedItemChanged(SlotNumber, ItemData);
			bSetSlot = true;
		}
	}

	if (bSetSlot)
	{
		return true;
	}

	// If ItemData is null or ItemData already exists, 
	// we remove the old slot.
	PlayerInventory.Remove(SlotNumber);
	NotifySlottedItemChanged(SlotNumber, nullptr);
	return false;
}

void AEdenValleyPlayerController::GetInventoryArray(TArray<UEdenValleyItemData*>& InventoryArray, EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::IT_Potion:
		for (UEdenValleyItemData* ItemData : PotionInventory) InventoryArray.Add(ItemData);
		break;
	case EItemType::IT_Skill:
		for (UEdenValleyItemData* ItemData : SkillInventory) InventoryArray.Add(ItemData);
		break;
	case EItemType::IT_Weapon:
		for (UEdenValleyItemData* ItemData : WeaponInventory) InventoryArray.Add(ItemData);
		break;
	case EItemType::IT_Other:
		for (UEdenValleyItemData* ItemData : OtherInventory) InventoryArray.Add(ItemData);
		break;
	}
}

void AEdenValleyPlayerController::GetInventoryArrayOfPurchasableItems(TArray<UEdenValleyItemData*>& PurchasableInventory)
{
	AddPurchasableItemsToInventory(PurchasableInventory, EItemType::IT_Weapon);
	AddPurchasableItemsToInventory(PurchasableInventory, EItemType::IT_Potion);
	AddPurchasableItemsToInventory(PurchasableInventory, EItemType::IT_Skill);
}

const TMap<int32, UEdenValleyItemData*> & AEdenValleyPlayerController::GetPlayerInventory() const
{
	return PlayerInventory;
}

const TArray<UEdenValleyItemData*> & AEdenValleyPlayerController::GetPotionInventory() const
{
	return PotionInventory;
}

const TArray<UEdenValleyItemData*> & AEdenValleyPlayerController::GetSkillInventory() const
{
	return SkillInventory;
}

const TArray<UEdenValleyItemData*> & AEdenValleyPlayerController::GetWeaponInventory() const
{
	return WeaponInventory;
}

const TArray<UEdenValleyItemData*>& AEdenValleyPlayerController::GetOtherInventory() const
{
	return OtherInventory;
}

void AEdenValleyPlayerController::NotifyInventoryLoaded()
{
	OnInventoryLoaded.Broadcast();
}

void AEdenValleyPlayerController::NotifyInventoryItemChanged(bool bAdded, UEdenValleyItemData* ItemData)
{
	InventoryItemChanged(bAdded, ItemData);
}

void AEdenValleyPlayerController::NotifySlottedItemChanged(int32 SlotNumber, UEdenValleyItemData* ItemData)
{
	OnSlottedItemChanged.Broadcast(SlotNumber, ItemData);
	InventorySlottedItemChanged(SlotNumber, ItemData);
}

bool AEdenValleyPlayerController::AddItemToInventory(TArray<UEdenValleyItemData*>& Inventory, UDataTable * DataTable, FName ID, int32 ItemCount)
{
	// Find the item in DB.
	FEdenValleyItem* Item = DataTable->FindRow<FEdenValleyItem>(ID, "");
	if (Item)
	{
		// Find the item in the inventory list.
		UEdenValleyItemData* ItemDataPtr = FindItemByID(Inventory, ID);
		if (ItemDataPtr)
		{
			// If this item already exists, we increment the number of instances.
			ItemDataPtr->Item.ItemCount += ItemCount;
		}
		else
		{
			// Add new item.
			ItemDataPtr = NewObject<UEdenValleyItemData>(this);
			ItemDataPtr->Item = *Item;
			ItemDataPtr->Item.ItemCount = ItemCount;
			Inventory.Add(ItemDataPtr);
			NotifyInventoryItemChanged(true, ItemDataPtr);
		}
		return true;
	}

	return false;
}

bool AEdenValleyPlayerController::RemoveItemFromInventory(TArray<UEdenValleyItemData*>& Inventory, UEdenValleyItemData * ItemData)
{
	int32 ItemIndex = Inventory.Find(ItemData);
	if (ItemIndex != INDEX_NONE)
	{
		Inventory.RemoveAt(ItemIndex);
		NotifyInventoryItemChanged(false, ItemData);
		return true;
	}

	return false;
}

UEdenValleyItemData * AEdenValleyPlayerController::FindItemByID(TArray<UEdenValleyItemData*> Inventory, FName ID)
{
	if (Inventory.Num() == 0) return nullptr;

	UEdenValleyItemData** ItemDataPtr = Inventory.FindByPredicate([&](UEdenValleyItemData* ItemToCompare)
	{
		if (ItemToCompare == nullptr) return false;
		return ItemToCompare->Item.ItemID == ID;
	});

	if (ItemDataPtr) return *ItemDataPtr;
	return nullptr;
}

void AEdenValleyPlayerController::AddPurchasableItemsToInventory(TArray<UEdenValleyItemData*>& PurchasableInventory, EItemType ItemType)
{
	AEdenValleyGameStateBase* GameState = Cast<AEdenValleyGameStateBase>(GetWorld()->GetGameState());
	UDataTable* TableDB = GameState->GetTableDB(ItemType); // Get table based on the item type.

	// Copy all rows into Items and save them in PurchasableInventory.
	TArray<FEdenValleyItem*> Items;
	TableDB->GetAllRows<FEdenValleyItem>("", Items);
	for (const FEdenValleyItem* Item : Items)
	{
		const int32 Price = Item->ItemPrice;
		if (Price > 0)
		{
			UEdenValleyItemData* ItemData = NewObject<UEdenValleyItemData>(this);
			ItemData->Item = *Item;
			PurchasableInventory.Add(ItemData);
		}
	}
}
