// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EdenValleyDamageExecution.generated.h"

/** A damage execution, which allows doing damage by combining a raw Damage number with AttackPower and DefensePower. */
UCLASS()
class EDENVALLEY_API UEdenValleyDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	/** Constructor. */
	UEdenValleyDamageExecution();

	/** Overrides. */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
		OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
