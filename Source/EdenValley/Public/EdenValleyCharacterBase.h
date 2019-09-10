// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "EdenValleyInventoryInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Abilities/EdenValleyAttributeSet.h"
#include "EdenValleyCharacterBase.generated.h"

/** Delegate called when character uses skills. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkill);

/** Base class for Characters. */
UCLASS(config = Game)
class EDENVALLEY_API AEdenValleyCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	/** Constructor. */
	AEdenValleyCharacterBase();

	/** Overrides. */
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	/** Implement IAbilitySystemInterface. */
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Delegate to be assign in BP. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Abilities)
	FOnSkill SkillDelegate;

	/** Returns currents health. It will bee 0 if dead. */
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual float GetHealth() const;

	/** Returns max health. */
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual float GetMaxHealth() const;

	/** Returns current mana. */
	UFUNCTION(BlueprintCallable, Category = Mana)
	virtual float GetMana() const;

	/** Returns max mana. */
	UFUNCTION(BlueprintCallable, Category = Mana)
	virtual float GetMaxMana() const;

	/** Returns the character level that is passed to the ability system. */
	UFUNCTION(BlueprintCallable, Category = Level)
	virtual int32 GetCharacterLevel() const;

	/** Modifies the character level. This may change abilities. Returns true on success */
	UFUNCTION(BlueprintCallable, Category = Level)
	virtual bool SetCharacterLevel(int32 NewLevel);

	/** Returns if character is still alive. */
	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive();

	/** Return by reference a list of active abilities that match match the specified tags. */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray< UAbilitySystemComponent*>& ActiveAbilities);
	
	/** Returns by reference a list of active abilities bound to the item slot. */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	void GetActiveAbilitiesWithItemSlot(int32 SlotNumber, TArray<UGameplayAbility*>& ActiveAbilities);

	/** Attempts to activate all abilities that match the specified tags. */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation);

	/** Attempts to activate any ability in the specified item slot. */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool ActivateAbilitiesWithItemSlot(int32 SlotNumber, bool bAllowRemoteActivation);

	/** Returns total time and remaining time for cooldown tags. Returns false if no active cooldowns found. */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

protected:	
	/** The level of this character. */
	UPROPERTY(EditAnywhere, Category = Abilities)
	int32 CharacterLevel;

	/** List of abilities to be used by this actor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> AbilityToStartup;

	/** Map of slot to ability granted by slot. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<int32, FGameplayAbilitySpecHandle> SlottedAbilities;

	/** Passive gameplay effects applied on creation. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/** The component used to handle ability system interactions. */
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComp;

	/** List of attributes modified by the ability system. */
	UPROPERTY()
	UEdenValleyAttributeSet* AttributeSet;

	/** Cached pointer to the inventory source for this character. */
	UPROPERTY()
	TScriptInterface<IEdenValleyInventoryInterface> InventorySource;

	/** If true we have initialized our abilities. */
	UPROPERTY()
	bool bAbilitiesInitilized;

	/** If true, abilities are ignored. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Abilities)
	bool bBlockAbilities;

	/** Returns true if this character can use any ability. */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool CanUseAnyAbililty();

	/** Returns true if this character is using some skill. */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool IsUsingSkill();

	/** Called when character takes damage. Implemented in blueprint.*/
	UFUNCTION(BlueprintImplementableEvent, Category = CharacterBase, Meta = (DisplayName = "OnDamageChanged"))
	void OnDamageChanged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, 
		AEdenValleyCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	/** Called when health is changed. Implemented in blueprint. */
	UFUNCTION(BlueprintImplementableEvent, Category = CharacterBase, Meta = (DisplayName = "OnHealthChanged"))
	void OnHealthChanged(float HealthAmount, const struct FGameplayTagContainer& EventTags);

	/** Called when mana is changed. Implemented in blueprint. */
	UFUNCTION(BlueprintImplementableEvent, Category = CharacterBase, Meta = (DisplayName = "OnManaChanged"))
	void OnManaChanged(float ManaAmount, const struct FGameplayTagContainer& EventTags);

	/** Called when slotted item change. Bound to delegate to interface. */
	UFUNCTION()
	void OnItemSlotChanged(int32 SlotNumber, UEdenValleyItemData* ItemData);

	/** Updates slotted item abilities. */
	UFUNCTION()
	void RefreshSlottedGameplayAbilities();

	/** Apply the startup gameplay abilities. */
	void AddStartupGameplayAbilities();

	/** Attempts to remove all startup gameplay abilities. */
	void RemoveStartupGameplayAbilities();

	/** Adds slotted item abilities if needed */
	void AddSlottedGameplayAbilities();

	/** Fills the map with ability specs based on the inventory. */
	void FillSlottedAbilitySpec(TMap<int32, FGameplayAbilitySpec>& SlottedAbilitySpec);

	/** Remove slotted gameplay abilities, if force is false it only removes invalid ones */
	void RemoveSlottedGameplayAbilities(bool bRemoveAll);

	/** Called from EdenValleyAttributeSet. These methods call BP events above. */
	virtual void HandleDamageChanged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, 
		AEdenValleyCharacterBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Friended to allow access this class to EdenValleyAttributeSet.
	friend UEdenValleyAttributeSet;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
