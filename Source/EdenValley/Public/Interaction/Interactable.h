// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

/** Serves as base class for every interactable actor in game. */
UCLASS()
class EDENVALLEY_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void Interact(APlayerController* Controller);

	virtual void Interact_Implementation(APlayerController* Controller);

	/** Name of the actor. */
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	/** Action to be carried out. */
	UPROPERTY(EditDefaultsOnly)
	FString Action;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	FString GetInteractText() const;
};
