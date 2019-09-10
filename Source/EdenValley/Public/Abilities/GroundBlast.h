// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GroundBlast.generated.h"

class UDecalComponent;
class USceneComponent;

/** Ability target actor to apply Ground Blast ability. */
UCLASS()
class EDENVALLEY_API AGroundBlast : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	/** Constructor. */
	AGroundBlast();

	/** Overrides. */
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundSelect")
	float Radius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundBlast")
	UDecalComponent* Decal;

	UFUNCTION(BlueprintCallable, Category = "GroundSelect")
	bool GetPlayerLookingPoint(FVector& OutViewPoint);
};
