// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenValleyGameStateBase.h"
#include "Items/EItemType.h"

AEdenValleyGameStateBase::AEdenValleyGameStateBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_PotionDB(TEXT("DataTable'/Game/Data/PotionDB.PotionDB'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_SkillDB(TEXT("DataTable'/Game/Data/SkillDB.SkillDB'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_WeaponDB(TEXT("DataTable'/Game/Data/WeaponDB.WeaponDB'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_OtherDB(TEXT("DataTable'/Game/Data/OtherDB.OtherDB'"));

	PotionDB = BP_PotionDB.Object;
	SkillDB = BP_SkillDB.Object;
	WeaponDB = BP_WeaponDB.Object;
	OtherDB = BP_OtherDB.Object;
}

UDataTable * AEdenValleyGameStateBase::GetPotionDB() const
{
	return PotionDB;
}

UDataTable * AEdenValleyGameStateBase::GetSkillDB() const
{
	return SkillDB;
}

UDataTable * AEdenValleyGameStateBase::GetWeaponDB() const
{
	return WeaponDB;
}

UDataTable * AEdenValleyGameStateBase::GetOtherDB() const
{
	return OtherDB;
}

UDataTable * AEdenValleyGameStateBase::GetTableDB(EItemType ItemType) const
{
	UDataTable* TableDB = nullptr;
	switch (ItemType)
	{
	case EItemType::IT_Weapon:
		TableDB = WeaponDB;
		break;
	case EItemType::IT_Potion:
		TableDB = PotionDB;
		break;
	case EItemType::IT_Skill:
		TableDB = SkillDB;
		break;
	case EItemType::IT_Other:
		TableDB = OtherDB;
		break;
	}
	return TableDB;
}