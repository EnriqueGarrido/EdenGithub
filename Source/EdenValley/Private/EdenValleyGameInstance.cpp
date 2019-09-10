// Fill out your copyright notice in the Description page of Project Settings.

#include "EdenValleyGameInstance.h"
#include "SaveGame/EdenValleySaveGame.h"
#include "Kismet/GameplayStatics.h"

UEdenValleyGameInstance::UEdenValleyGameInstance()
	: SaveSlot(TEXT(""))
	, SaveUserIndex(0)
{
}

UEdenValleySaveGame * UEdenValleyGameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void UEdenValleyGameInstance::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

bool UEdenValleyGameInstance::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		CurrentSaveGame = Cast<UEdenValleySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		return true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<UEdenValleySaveGame>(UGameplayStatics::CreateSaveGameObject(UEdenValleySaveGame::StaticClass()));
		return false;
	}
}

bool UEdenValleyGameInstance::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		return UGameplayStatics::SaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex);
	}
	return false;
}

void UEdenValleyGameInstance::ResetSaveGame()
{
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}
