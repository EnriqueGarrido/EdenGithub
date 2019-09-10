// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EdenValleyAttributeSet.generated.h"

// Uses macros from AttributeSet.h to define automatically all helper functions for accessing and initializing attributes,
// avoiding having to manually write these functions
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** Subclass of AttributeSet to define attributes used in the game. */
UCLASS()
class EDENVALLEY_API UEdenValleyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	/** Constructor */
	UEdenValleyAttributeSet();

	/** Overrides */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Current Health. When 0 we expect character to die. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEdenValleyAttributeSet, Health)

	/** Max Health. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEdenValleyAttributeSet, MaxHealth)

	/** Current Mana. Used to execute special abilities. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UEdenValleyAttributeSet, Mana)

	/** Max Mana. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UEdenValleyAttributeSet, MaxMana)

	/** AttackPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UEdenValleyAttributeSet, AttackPower)

	/** Base Damage is divided by DefensePower to get actual damage done, so 1.0 means no bonus */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UEdenValleyAttributeSet, DefensePower)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UEdenValleyAttributeSet, Damage)

protected:
	/** 
	 * Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. 
	 * (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before).
	 */
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, 
		float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
};
