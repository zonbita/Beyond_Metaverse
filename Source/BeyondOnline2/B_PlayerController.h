// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../BeyondOnline2/HexGridManager.h"
#include "./Actors/ChunkManager.h" 
#include "B_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BEYONDONLINE2_API AB_PlayerController : public APlayerController
{
	GENERATED_BODY()

	TArray<FIntVector2> Direction2D = {
	{0, -1},
	{-1, 0},
	{-1, 1},
	{1, 0},
	{0, 1},
	{1, 1}
	};

	
	float RangeX = 1778.0, RangeX2 = 889, RangeY = 1458.0, RangeY2 = 729;


	FIntVector2 CalculatorRegionforOwnClient();

	UFUNCTION(BlueprintCallable)
		TArray<FString> GetRegionsAroundPlayer();
	
	UFUNCTION(BlueprintCallable)
		void GetRegionPlayer(int& x, int& y);

	UFUNCTION(BlueprintCallable)
		TArray<FRegionData> RegionsAroundPlayerNeedSpawn(TArray<FString> Regions);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> RegionsName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AHexGridManager* HexGridManager;

	UFUNCTION(BlueprintCallable)
		void SetOwnerChunkManager(APawn* NewOwnerPawn, FString PlayerName);

	UFUNCTION(BlueprintCallable)
		bool TraceChunk(const FVector& Location, ETraceTypeQuery TraceChannel, FString PlayerName, FString& RegionName, FString& OwnerName, FString& TriangleName);


};
