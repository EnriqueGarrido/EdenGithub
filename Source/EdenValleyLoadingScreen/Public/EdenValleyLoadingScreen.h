// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModuleInterface.h"
#include "Modules/ModuleManager.h"

/** Module interface for this game's loading screens */
class IEdenValleyLoadingScreenModule : public IModuleInterface
{
public:
	/** Loads the module so it can be turned on */
	static inline IEdenValleyLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IEdenValleyLoadingScreenModule>("EdenValleyLoadingScreen");
	}

	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) = 0;

	/** Stops the loading screen */
	virtual void StopInGameLoadingScreen() = 0;
};