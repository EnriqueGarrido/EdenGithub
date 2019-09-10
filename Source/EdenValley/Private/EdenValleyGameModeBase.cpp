// Fill out your copyright notice in the Description page of Project Settings.

#include "EdenValleyGameModeBase.h"
#include "EdenValleyCharacterBase.h"
//#include "EdenValleyPlayerController.h"

AEdenValleyGameModeBase::AEdenValleyGameModeBase()
{
	//PlayerControllerClass = AEdenValleyPlayerController::StaticClass();
	DefaultPawnClass = AEdenValleyCharacterBase::StaticClass();
}