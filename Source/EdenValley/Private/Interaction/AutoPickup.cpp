// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction\AutoPickup.h"
#include "EdenValleyPlayerController.h"

// Sets default values
AAutoPickup::AAutoPickup() : ItemID(""), ItemType(EItemType::IT_None)
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);
}

void AAutoPickup::Collect_Implementation(APlayerController * Controller)
{
	AEdenValleyPlayerController* PlayerController = Cast<AEdenValleyPlayerController>(Controller);
	if (PlayerController->AddItemToInventoryByID(ItemType, ItemID, 1))
	{
		Destroy();
	}
}

FName AAutoPickup::GetItemID()
{
	return ItemID;
}
