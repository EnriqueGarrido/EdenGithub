// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/FEdenValleyItem.h"
#include "FEdenValleyWeaponItem.generated.h"

/** Native base class for weapons. */
USTRUCT(BlueprintType)
struct FEdenValleyWeaponItem : public FEdenValleyItem
{
	GENERATED_BODY()

public:
	/** Constructor. */
	FEdenValleyWeaponItem()
	{
		ItemType = EItemType::IT_Weapon;
	}

	/** Weapon actor to be spawned. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> WeaponActor;
};