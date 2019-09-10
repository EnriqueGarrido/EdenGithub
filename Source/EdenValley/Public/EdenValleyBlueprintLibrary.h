// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayAbility.h"
#include "EdenValleyBlueprintLibrary.generated.h"

/** Game-specific blueprint library. */
UCLASS()
class EDENVALLEY_API UEdenValleyBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
public:
	/** 
	 * Show the native loading screen, such as on a map transfer. If bPlayUntilStopped is false, it will be displayed for PlayTime and 
	 * automatically stop.
	 */
	UFUNCTION(BlueprintCallable, Category = Loading)
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	/** Turns off the native loading screen if it is visible. This must be called if bPlayUntilStopped was true. */
	UFUNCTION(BlueprintCallable, Category = Loading)
	static void StopLoadingScreen();

	/** Apply Gameplay Effect to target data from outside of an ability. */
	UFUNCTION(BlueprintCallable, Category = Ability)
	static void ApplyGESpecHandleToTargetDataSpecsHandle(const FGameplayEffectSpecHandle& GESpecHandle, 
		const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	/** Get all save slot names from save file located at Saved/SaveGames. */
	UFUNCTION(BlueprintCallable, Category = Save)
	static TArray<FString> GetAllSaveGameSlotNames();
};
