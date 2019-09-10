// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/EItemType.h"
#include "AutoPickup.generated.h"

/** Uses Interactable as base clase to pickup items automatically. */
UCLASS()
class EDENVALLEY_API AAutoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoPickup();

	UFUNCTION(BlueprintNativeEvent)
	void Collect(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, Category = AutoPickup)
	virtual void Collect_Implementation(APlayerController* Controller);

	/** Returns identifier of this item. */
	FName GetItemID();

protected:
	/** Pointer to static mesh component. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PickupMesh;
	
	/** Identifier of this item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	/** Type of this item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
};
