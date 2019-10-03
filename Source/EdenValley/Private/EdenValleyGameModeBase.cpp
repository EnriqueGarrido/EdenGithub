// Fill out your copyright notice in the Description page of Project Settings.

#include "EdenValleyGameModeBase.h"
#include "EdenValleyCharacterBase.h"

AEdenValleyGameModeBase::AEdenValleyGameModeBase()
{
	DefaultPawnClass = AEdenValleyCharacterBase::StaticClass();
}