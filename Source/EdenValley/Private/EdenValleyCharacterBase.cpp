// Fill out your copyright notice in the Description page of Project Settings.

#include "EdenValleyCharacterBase.h"
#include "EngineMinimal.h"
#include "GameplayTagsManager.h"
#include "AbilitySystemComponent.h"

// Sets default values
AEdenValleyCharacterBase::AEdenValleyCharacterBase()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule.
	GetCapsuleComponent()->InitCapsuleSize(45.0f, 96.0f);

	// Create ability system component.
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Create the attribute set
	AttributeSet = CreateDefaultSubobject<UEdenValleyAttributeSet>(TEXT("AttributeSet"));

	CharacterLevel = 1;
	bAbilitiesInitilized = false;
	bBlockAbilities = false;
}

void AEdenValleyCharacterBase::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	InventorySource = NewController;

	if (InventorySource)
	{
		// Register delegate from PlayerController.
		InventorySource->GetSlottedItemChangedDelegate().AddDynamic(this, &AEdenValleyCharacterBase::OnItemSlotChanged);
		InventorySource->GetInventoryLoadedDelegate().AddDynamic(this, &AEdenValleyCharacterBase::RefreshSlottedGameplayAbilities);
	}

	// Initialize abilities.
	if (AbilitySystemComp)
	{
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

void AEdenValleyCharacterBase::UnPossessed()
{
	Super::UnPossessed();

	if (InventorySource)
	{
		// Unregister delegate from PlayerController.
		InventorySource->GetSlottedItemChangedDelegate().RemoveDynamic(this, &AEdenValleyCharacterBase::OnItemSlotChanged);
		InventorySource->GetInventoryLoadedDelegate().RemoveDynamic(this, &AEdenValleyCharacterBase::RefreshSlottedGameplayAbilities);
	}

	InventorySource = nullptr;
}

UAbilitySystemComponent * AEdenValleyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

float AEdenValleyCharacterBase::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AEdenValleyCharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AEdenValleyCharacterBase::GetMana() const
{
	return AttributeSet->GetMana();
}

float AEdenValleyCharacterBase::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

int32 AEdenValleyCharacterBase::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool AEdenValleyCharacterBase::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0)
	{
		// Our level changed so we need to refresh abilities.
		RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		AddStartupGameplayAbilities();

		return true;
	}
	return false;
}

bool AEdenValleyCharacterBase::IsAlive()
{
	return GetHealth() > 0;
}

bool AEdenValleyCharacterBase::CanUseAnyAbililty()
{
	bool bCanUseAnyAbility = false;

	if (!IsUsingSkill() && IsAlive()) bCanUseAnyAbility = true;
	return bCanUseAnyAbility;
}

bool AEdenValleyCharacterBase::IsUsingSkill()
{
	bool bUsingSkill = false;

	const FName Tag = FName(TEXT("Skill")); // Get any skill.
	FGameplayTag SkillTag = UGameplayTagsManager::Get().RequestGameplayTag(Tag);
	FGameplayTagContainer AbilityTags = FGameplayTagContainer(SkillTag);
	TArray<UAbilitySystemComponent*> ActiveAbilities;
	GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);

	if (ActiveAbilities.Num() > 0) bUsingSkill = true;
	return bUsingSkill;
}

void AEdenValleyCharacterBase::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UAbilitySystemComponent*>& ActiveAbilities)
{
	if (AbilitySystemComp)
	{
		TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
		AbilitySystemComp->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate, false);

		// Iterate the list of all ability specs.
		for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
		{
			// Iterate all instances on this ability spec.
			TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				ActiveAbilities.Add(AbilitySystemComp);
			}
		}
	}
}

void AEdenValleyCharacterBase::GetActiveAbilitiesWithItemSlot(int32 SlotNumber, TArray<UGameplayAbility*>& ActiveAbilities)
{
	FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(SlotNumber);

	if (FoundHandle && AbilitySystemComp)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComp->FindAbilitySpecFromHandle(*FoundHandle);

		if (FoundSpec)
		{
			TArray<UGameplayAbility*> AbilityInstances = FoundSpec->GetAbilityInstances();

			// Find all ability instances executed from this slot.
			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}
}

bool AEdenValleyCharacterBase::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComp && !bBlockAbilities)
	{
		return AbilitySystemComp->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}
	return false;
}

bool AEdenValleyCharacterBase::ActivateAbilitiesWithItemSlot(int32 SlotNumber, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(SlotNumber);

	if (FoundHandle && AbilitySystemComp && !bBlockAbilities)
	{
		return AbilitySystemComp->TryActivateAbility(*FoundHandle, bAllowRemoteActivation);
	}
	return false;
}

bool AEdenValleyCharacterBase::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration)
{
	if (AbilitySystemComp && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.0f;
		CooldownDuration = 0.0f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray<TPair<float, float>> DurationAndTimeRemaining = AbilitySystemComp->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d"), DurationAndTimeRemaining.Num());
			int32 BestIndex = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Index = 1; Index < DurationAndTimeRemaining.Num(); ++Index)
			{
				float Duration = DurationAndTimeRemaining[Index].Key;
				if (Duration > LongestTime)
				{
					LongestTime = Duration;
					BestIndex = Index;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIndex].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIndex].Value;

			return true;
		}
	}
	return false;
}

void AEdenValleyCharacterBase::OnItemSlotChanged(int32 SlotNumber, UEdenValleyItemData * ItemData)
{
	RefreshSlottedGameplayAbilities();
}

void AEdenValleyCharacterBase::AddStartupGameplayAbilities()
{
	check(AbilitySystemComp);

	if (!bAbilitiesInitilized)
	{
		if (AbilityToStartup.Num() > 0)
		{
			// Register start up abilities.
			for (TSubclassOf<UGameplayAbility> AbilityClass : AbilityToStartup)
			{
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityClass, GetCharacterLevel(), INDEX_NONE, this));
			}
		}

		if (PassiveGameplayEffects.Num() > 0)
		{
			// Apply passives.
			for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				FGameplayEffectSpecHandle NewHandle = AbilitySystemComp->MakeOutgoingSpec(GameplayEffect, CharacterLevel, EffectContext);
				if (NewHandle.IsValid())
				{
					FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(
						*NewHandle.Data.Get(), AbilitySystemComp);
				}
			}
		}

		bAbilitiesInitilized = true;
	}
}

void AEdenValleyCharacterBase::RemoveStartupGameplayAbilities()
{
	check(AbilitySystemComp);

	if (bAbilitiesInitilized)
	{
		// Remove any abilities added from a previous call.
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComp->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && AbilityToStartup.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}
		
		// Do in two passes so the removal happens after we have the full list.
		for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
		{
			AbilitySystemComp->ClearAbility(AbilitiesToRemove[i]);
		}

		// Remove all of the passive gameplay effects that were applied by this character.
		FGameplayEffectQuery Query;
		Query.EffectSource = this; // Matches on GameplayEffects which come from this source.
		AbilitySystemComp->RemoveActiveEffects(Query);

		RemoveSlottedGameplayAbilities(true);

		bAbilitiesInitilized = false;
	}
}

void AEdenValleyCharacterBase::RefreshSlottedGameplayAbilities()
{
	if (bAbilitiesInitilized)
	{
		// Refresh any valid abilities and adds new ones.
		RemoveSlottedGameplayAbilities(false);
		AddSlottedGameplayAbilities();
	}
}

void AEdenValleyCharacterBase::AddSlottedGameplayAbilities()
{
	TMap<int32, FGameplayAbilitySpec> SlottedAbilitySpecs;
	FillSlottedAbilitySpec(SlottedAbilitySpecs);

	// Now add abilities if needed.
	for (const TPair<int32, FGameplayAbilitySpec>& SpecPair : SlottedAbilitySpecs)
	{
		FGameplayAbilitySpecHandle& SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);

		if (!SpecHandle.IsValid())
		{
			SpecHandle = AbilitySystemComp->GiveAbility(SpecPair.Value);
		}
	}
}

void AEdenValleyCharacterBase::FillSlottedAbilitySpec(TMap<int32, FGameplayAbilitySpec>& SlottedAbilitySpec)
{
	if (InventorySource)
	{
		const TMap<int32, UEdenValleyItemData*>& PlayerInventory = InventorySource->GetPlayerInventory();

		for (const TPair<int32, UEdenValleyItemData*>& ItemDataPair : PlayerInventory)
		{
			UEdenValleyItemData* ItemData = ItemDataPair.Value;

			// Use the character level as default.
			int32 AbilityLevel = GetCharacterLevel();

			if (ItemData && ItemData->Item.ItemType == EItemType::IT_Weapon)
			{
				// Override the ability level to use the data from the slotted item.
				AbilityLevel = ItemData->Item.AbilityLevel;
			}

			if (ItemData && ItemData->Item.GrantedAbility)
			{
				// This will override anything from default.
				SlottedAbilitySpec.Add(ItemDataPair.Key, FGameplayAbilitySpec(ItemData->Item.GrantedAbility, AbilityLevel, INDEX_NONE, ItemData));
			}
		}
	}
}

void AEdenValleyCharacterBase::RemoveSlottedGameplayAbilities(bool bRemoveAll)
{
	TMap<int32, FGameplayAbilitySpec> SlottedAbilitySpecs;

	if (!bRemoveAll)
	{
		// Fill in map, so we can compare.
		FillSlottedAbilitySpec(SlottedAbilitySpecs);
	}

	for (TPair<int32, FGameplayAbilitySpecHandle>& ExistingPair : SlottedAbilities)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComp->FindAbilitySpecFromHandle(ExistingPair.Value);
		bool bShouldRemove = bRemoveAll || !FoundSpec;

		if (!bShouldRemove)
		{
			// Need to check desired ability specs, if we got here FoundSpec is valid.
			FGameplayAbilitySpec* DesiredSpec = SlottedAbilitySpecs.Find(ExistingPair.Key);

			if (!DesiredSpec || DesiredSpec->Ability != FoundSpec->Ability || DesiredSpec->SourceObject != FoundSpec->SourceObject)
			{
				bShouldRemove = true;
			}
		}

		if (bShouldRemove)
		{
			if (FoundSpec)
			{
				// Need to remove registered ability.
				AbilitySystemComp->ClearAbility(ExistingPair.Value);
			}

			// Make sure handle is cleared even if ability wasn't found.
			ExistingPair.Value = FGameplayAbilitySpecHandle();
		}
	}
}

void AEdenValleyCharacterBase::HandleDamageChanged(float DamageAmount, const FHitResult & HitInfo, const FGameplayTagContainer & DamageTags, AEdenValleyCharacterBase * InstigatorCharacter, AActor * DamageCauser)
{
	if (bAbilitiesInitilized)
	{
		OnDamageChanged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
	}
}

void AEdenValleyCharacterBase::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
	if (bAbilitiesInitilized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void AEdenValleyCharacterBase::HandleManaChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
	if (bAbilitiesInitilized)
	{
		OnManaChanged(DeltaValue, EventTags);
	}
}

// Called every frame
void AEdenValleyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}