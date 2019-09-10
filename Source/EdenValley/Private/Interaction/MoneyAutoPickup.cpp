// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction\MoneyAutoPickup.h"
#include "EdenValleyPlayerController.h"

AMoneyAutoPickup::AMoneyAutoPickup()
{
	Super::ItemID = FName("Money");
	Value = 1;
}

void AMoneyAutoPickup::Collect_Implementation(APlayerController * Controller)
{
	AEdenValleyPlayerController* PlayerController = Cast<AEdenValleyPlayerController>(Controller);
	PlayerController->SetMoney(Value);
	Destroy();
}

int32 AMoneyAutoPickup::GetValue()
{
	return Value;
}
