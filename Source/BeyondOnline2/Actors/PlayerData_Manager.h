// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PlayerData_Manager.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		TArray<FString> OwnerChunks;
};

UCLASS()
class BEYONDONLINE2_API APlayerData_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerData_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerData PlayerData;


	UFUNCTION(BlueprintCallable, Category = "Player Data")
		static void SavePlayerData(const FString& PlayerName, const FPlayerData& BPlayerData);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
		static bool LoadPlayerData(const FString& PlayerName, FPlayerData& OutPlayerData);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
		TArray<FString> GetAllPlayerDataFiles();

	UFUNCTION(BlueprintCallable, Category = "Player Data")
		bool FindPlayerDataFileByName(const FString& PlayerName, FString& OutFilePath);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
		void FindPlayerData(const FString& PlayerName);
};
