// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenValleyBlueprintLibrary.h"
#include "EdenValleyLoadingScreen.h"
#include "FileHelper.h"
#include "Paths.h"
#include "PlatformFile.h"
#include "PlatformFilemanager.h"

UEdenValleyBlueprintLibrary::UEdenValleyBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEdenValleyBlueprintLibrary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	IEdenValleyLoadingScreenModule& LoadingScreenModule = IEdenValleyLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UEdenValleyBlueprintLibrary::StopLoadingScreen()
{
	IEdenValleyLoadingScreenModule& LoadingScreenModule = IEdenValleyLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

void UEdenValleyBlueprintLibrary::ApplyGESpecHandleToTargetDataSpecsHandle(const FGameplayEffectSpecHandle& GESpecHandle, const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	for (TSharedPtr<FGameplayAbilityTargetData> Data : TargetDataHandle.Data)
	{
		Data->ApplyGameplayEffectSpec(*GESpecHandle.Data.Get());
	}
}

TArray<FString> UEdenValleyBlueprintLibrary::GetAllSaveGameSlotNames()
{
	class FFindSavesVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		/** Constructor. */
		FFindSavesVisitor() {}
		
		TArray<FString> SavesFound;

		virtual bool Visit(const TCHAR* FileNameOrDirectory, bool bIsDirectory)
		{
			if (!bIsDirectory)
			{
				FString FullFilePath(FileNameOrDirectory);
				if (FPaths::GetExtension(FullFilePath) == TEXT("sav"))
				{
					FString CleanFileName = FPaths::GetBaseFilename(FullFilePath);
					CleanFileName = CleanFileName.Replace(TEXT("sav"), TEXT(""));
					SavesFound.Add(CleanFileName);
				}
			}
			return true;
		}
	};

	TArray<FString> SaveGameSlotNames;
	const FString SavesFolderPath = FPaths::ProjectSavedDir() + TEXT("SaveGames");

	if (!SavesFolderPath.IsEmpty())
	{
		FFindSavesVisitor Visitor;
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*SavesFolderPath, Visitor);
		SaveGameSlotNames = Visitor.SavesFound;
	}
	return SaveGameSlotNames;
}

bool UEdenValleyBlueprintLibrary::LoadStringFile(FString& Result, FString FolderName, FString FileName)
{
	const FString FolderPath = FPaths::ProjectDir() + FolderName;
	if (FolderPath.IsEmpty())
	{
		return false;
	}

	const FString FilePath = FolderPath + FileName;
	if (!FPaths::FileExists(FilePath))
	{
		return false;
	}

	if (FFileHelper::LoadFileToString(Result, *FilePath))
	{
		return true;
	}

	return false;
}
