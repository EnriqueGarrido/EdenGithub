// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/GameStateBase.h"
#include "Items/EItemType.h"
#include "EdenValleyGameStateBase.generated.h"

/** Base class for holding all databases. */
UCLASS()
class EDENVALLEY_API AEdenValleyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	/** Constructor. */
	AEdenValleyGameStateBase();

	/** Getter for all DB. */
	UFUNCTION(BlueprintCallable, Category = DB)
	UDataTable* GetPotionDB() const;

	UFUNCTION(BlueprintCallable, Category = DB)
	UDataTable* GetSkillDB() const;

	UFUNCTION(BlueprintCallable, Category = DB)
	UDataTable* GetWeaponDB() const;

	UFUNCTION(BlueprintCallable, Category = DB)
	UDataTable* GetOtherDB() const;

	UFUNCTION(BlueprintCallable, Category = DB)
	UDataTable* GetTableDB(EItemType ItemType) const;

protected:
	/** Pointer to Potion DB. */
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PotionDB;

	/** Pointer to Skill DB. */
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* SkillDB;

	/** Pointer to Weapon DB. */
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* WeaponDB;

	/** Pointer to Other DB. */
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* OtherDB;
};
