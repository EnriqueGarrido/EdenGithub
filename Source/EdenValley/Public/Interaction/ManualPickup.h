// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Interactable.h"
#include "Items/EItemType.h"
#include "ManualPickup.generated.h"

/** Uses Interactable as base clase to pickup items manually. */
UCLASS()
class EDENVALLEY_API AManualPickup : public AInteractable
{
	GENERATED_BODY()
	
public:
	/** Constructor. */
	AManualPickup();

	UFUNCTION(BlueprintCallable, Category = ManualPickup)
	void Interact_Implementation(APlayerController* Controller) override;

	/** Returns the identifier of this item. */
	FName GetItemID();

protected:
	/* Pointer to static mesh component. */
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	/** Identifier of this item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	/** Type of this item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
};
