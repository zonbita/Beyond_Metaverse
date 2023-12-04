// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/PlayerData_Manager.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"


// Sets default values
APlayerData_Manager::APlayerData_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlayerData_Manager::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerData_Manager::SavePlayerData(const FString& PlayerName, const FPlayerData& PlayerData)
{
    FString PlayerDataJson;
    FJsonObjectConverter::UStructToJsonObjectString(PlayerData, PlayerDataJson);

    FString Path = FPaths::ProjectDir() + TEXT("/PlayerData/");

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    if (!PlatformFile.DirectoryExists(*Path))
        PlatformFile.CreateDirectory(*Path);

    FString SavePath = Path + PlayerName + TEXT(".json");
    FFileHelper::SaveStringToFile(PlayerDataJson, *SavePath);
}

bool APlayerData_Manager::LoadPlayerData(const FString& PlayerName, FPlayerData& OutPlayerData)
{
    FString LoadPath = FPaths::ProjectDir() + "/PlayerData/" + PlayerName + TEXT(".json");
    FString PlayerDataJson;
    if (!FFileHelper::LoadFileToString(PlayerDataJson, *LoadPath))
    {
        return false; // Failed to load the file
    }

    return FJsonObjectConverter::JsonObjectStringToUStruct(PlayerDataJson, &OutPlayerData, 0, 0);
}


TArray<FString> APlayerData_Manager::GetAllPlayerDataFiles()
{
    TArray<FString> PlayerDataFiles;
    FString PlayerDataFolderPath = FPaths::ProjectDir() + "/PlayerData/";

    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFiles(PlayerDataFiles, *PlayerDataFolderPath, TEXT("*.json"));

    return PlayerDataFiles;
}

bool APlayerData_Manager::FindPlayerDataFileByName(const FString& PlayerName, FString& OutFilePath)
{
    FString SearchPattern = FPaths::ProjectDir() + "/PlayerData/" + PlayerName + TEXT(".json");

    IFileManager& FileManager = IFileManager::Get();
    TArray<FString> FoundFiles;
    FileManager.FindFiles(FoundFiles, *SearchPattern, true, false);

    if (FoundFiles.Num() > 0)
    {
        OutFilePath = FoundFiles[0];
        return true;
    }

    return false;
}

void APlayerData_Manager::FindPlayerData(const FString& PlayerName)
{

    // Get all JSON files in the "PlayerData" folder
    TArray<FString> AllPlayerDataFiles = GetAllPlayerDataFiles();

    // Print the names of all the JSON files
    for (const FString& PlayerDataFile : AllPlayerDataFiles)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerData file: %s"), *PlayerDataFile);
    }

}
