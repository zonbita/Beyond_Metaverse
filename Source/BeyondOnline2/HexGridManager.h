// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Math/Color.h"
#include "GameFramework/Actor.h"
#include "Actors/Actor_Hexagon.h"
#include "BeyondOnline2.h"
#include "Containers/Map.h"
#include "./FastNoiseLite.h"
#include "HexagonHISM.h"
#include "Actors/SpawnRandomManager.h"
#include "Containers/Map.h"
#include "Internationalization/Text.h"
#include "HexGridManager.generated.h"



class UFastNoiseWrapper;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfter, FTransform, Transform);


UCLASS(Config = Game)
class BEYONDONLINE2_API AHexGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGridManager(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	TArray<FIntVector2> Direction = {
	{0, -1},
	{-1, 0},
	{-1, 1},
	{1, 0},
	{0, 1},
	{1, 1}
	};


	UPROPERTY(BlueprintReadWrite, Category = "Player Data")
		TMap<FString, FString> TMap_String;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<UHexagonHISM> HISM1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<UHexagonHISM> HISM2;

	bool CheckJsonFileExists(const FString& FilePath);

	UFUNCTION(BlueprintCallable)
		void SaveChunkDataToFile(const FString& FilePath, const FChunkData& ChunkData);

	UFUNCTION(BlueprintCallable)
		void SaveRegionDataToFile(const FString& FilePath, const FRegionData& RegionData);


	// Random Assets

	UPROPERTY(BlueprintReadWrite, Category = "SpawnRandomManager")
	ASpawnRandomManager* SpawnRandomManager;

	// Chunk Asset

	UFUNCTION(BlueprintCallable)
		void SaveChunkAssetsToFile(const FString& FilePath, const FChunkAssets& FChunkAssets);

	UFUNCTION(BlueprintCallable)
		void SaveAChunkAsset(FString Name, FString FileName, FTransform Transform, FAsset_Info Info, FCompountContainer CTag, int& Index);

	UFUNCTION(BlueprintCallable)
		void SaveARegionAsset(FString RegionName, FString ObjectName, FTransform Transform, FAsset_Info Info, FCompountContainer CTag, int& Index);

	UFUNCTION(BlueprintCallable)
		bool CustomizeARegionAsset(FString RegionName, FString NewRegionName, FString ObjectName, FTransform Transform, int Index, int& newIndex, bool& bOtherChunk);

	// Owner Chunk

	UFUNCTION(BlueprintCallable)
		TMap<FString, FString> LoadOwnerFromFile();

	UFUNCTION(BlueprintCallable)
		static void SaveOwnerToFile(const TMap<FString, FString> Array);

	UFUNCTION(BlueprintCallable)
		void GetOwnerMySelf(FString MyName, TArray<FString>& Output_Region);

	UPROPERTY(BlueprintReadWrite)
		TMap<FString, FString> OwnerRegion;

	UPROPERTY(BlueprintReadWrite)
		TArray<FString> MyRegion;

	UFUNCTION(BlueprintCallable)
		void SetMyRegionUnique(TArray<FString> Regions);




	// Load Chunk

	FChunkData LoadChunkDataFromFile(const FString& FilePath);
	FChunkAssets LoadChunkAssetsFromFile(const FString& FilePath);

	FRegionData LoadRegionDataFromFile(const FString& FilePath);

	UFUNCTION(BlueprintCallable)
		bool RemoveGLTFSlotFromChunkAssets(const FString& RegionName, const FString& PlayerName, const FString& GLTFName, int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
		TArray<FChunkData> GetChunkDataWithChunkSlot(FVector Location, int number);

	void UpdateChunkDataWithAir(const FString& RegionName, int ChunkIndex, const int HISMID, const int BlockIndex);

	UFUNCTION(BlueprintCallable)
	void LoadConfigFromFile(FString FilePath, TMap<FString, FString>& ConfigData);

	UFUNCTION(BlueprintCallable)
		void FindTriangleInfo(const FString& FilePath, FChunkData& OutChunkData);

	UFUNCTION(BlueprintCallable)
		void CreateMap(int Size, int Height);

	UFUNCTION(BlueprintCallable)
		void LoadAllRegion(int Size);

	UFUNCTION(BlueprintCallable)
		void GenerateRegionChunkMesh(FRegionData RegionData);

	UFUNCTION(BlueprintCallable)
		FRegionData LoadAndGenerateARegion(const FString& Name);

	UFUNCTION(BlueprintCallable)
		UHexagonHISM* FindRegionFromClient(const FString& RegionName, const int& HISMID);

	UFUNCTION(BlueprintCallable)
		AChunkManager* CL_FindChunkManager(const FString& RegionName);

	UFUNCTION(BlueprintCallable)
		void CL_GenerateRegionMesh(FChunkData ChunkData, FString RegionName);

	UFUNCTION(BlueprintCallable)
		TArray<FString> FindAroundHexagons(FString Name);

	UFUNCTION(BlueprintCallable)
		FRegionData FindDataOfRegion(const FString& Name);

	UFUNCTION(BlueprintCallable)
		void GetBlocksARegion(FRegionData RD,TArray<FArray_unit8>& B);


	UFUNCTION(BlueprintCallable)
		void CL_CreateHexagon(FString Name,FChunkData ChunksData);

	UFUNCTION(BlueprintCallable)
		FChunkData GetAHexInfo(FString Names);


	UFUNCTION(BlueprintCallable)
		FChunkAssets GetHexAssets(FString Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Offset = 7.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Range = 220.45;
		//float Range = 440.9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RangeX = 1778.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RangeY = 1458.0;

	UPROPERTY(BlueprintAssignable)
		FAfter After;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSpawnAChunk, FRegionData, RegionData, FTransform, Transform, FString, OwnerChunk);

	// The multicast delegate instance that can be used to bind functions
	UPROPERTY(BlueprintAssignable, Category = "ChunkManager")
		FSpawnAChunk OnChunkSpawned;

	UPROPERTY(BlueprintReadWrite)
		TArray<FRegionData> RegionsData ;

	TArray<FRegionData>& GetRegionsData() { return RegionsData; }


private:

	UPROPERTY(EditAnywhere, Category = "Components")
		class USceneComponent* UScene;

	// Sqrt 1.7
	double s = FMath::Sqrt(3.f) / 2.f;

	// Subtract offset
	//double i = ((1 - s) + 1);

	

	FRegionData Cl_RegionData;

	float LX = 0.f;
	float LY = 0.f;

	//float RangeX = 2750.0, RangeY = 2150.0;

	FastNoiseLite* Noise;

	// Declare a TMap with FVector keys and FBlockData values
	TArray<FVector> Blocks;

	//void SpawnActorAsync(FVector Location);
	void AddInstanceAsync(UHexagonHISM* HISM, const FTransform& Transform, const FString& TriangleName, const FString& Hexagon_Name, const int& Number);
	void SetMeshAsync(UHexagonHISM* HISM);



};
