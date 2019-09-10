// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction\ManualPickup.h"
#include "EdenValleyPlayerController.h"

AManualPickup::AManualPickup() : ItemID(""), ItemType(EItemType::IT_None)
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);
}

void AManualPickup::Interact_Implementation(APlayerController * Controller)
{
	Super::Interact_Implementation(Controller);
	
	AEdenValleyPlayerController* PlayerController = Cast<AEdenValleyPlayerController>(Controller);
	if (PlayerController->AddItemToInventoryByID(ItemType, ItemID, 1))
	{
		Destroy();
	}
}

FName AManualPickup::GetItemID()
{
	return ItemID;
}
