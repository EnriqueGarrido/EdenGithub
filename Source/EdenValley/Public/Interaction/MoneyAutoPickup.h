// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/AutoPickup.h"
#include "MoneyAutoPickup.generated.h"

/** Special form of AutoPickup for collecting money. */
UCLASS()
class EDENVALLEY_API AMoneyAutoPickup : public AAutoPickup
{
	GENERATED_BODY()
	
public:
	AMoneyAutoPickup();

	void Collect_Implementation(APlayerController* Controller) override;

	/** Returns the value of this item. */
	UFUNCTION(BlueprintCallable)
	int32 GetValue();

protected:
	/** Value of this item we have collected. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money)
	int32 Value;
};
