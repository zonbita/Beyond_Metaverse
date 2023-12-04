// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BeyondOnline2.h"
#include "../HexagonHISM.h"
#include "./SpawnRandomManager.h"
#include "GameFramework/Actor.h"
#include "ChunkManager.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpawnTriangleDelegate, AChunkManager*, Chunk, FTransform, Location, FString, TriangleName, UHexagonHISM*, HISM);

UCLASS()
class BEYONDONLINE2_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();

	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UHexagonHISM> HISM1;

	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UHexagonHISM> HISM2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		float Range = 82.5;
	UPROPERTY(BlueprintReadWrite)
		float Offset = 1.5;
	UPROPERTY(BlueprintReadWrite)
		float Height = 43.0;
	void AddInstanceAsync(UHexagonHISM* HISM, const FTransform& Transform, const FString& TriangleName, const FString& Hexagon_Name, const int& ChunkIndex, const int& Number, const int& Block, const FAsset_Info& Info);

	void AddInstanceAsync2(UHexagonHISM* HISM,const FChunkInstance CI);

	bool CompletedHex = false;

	TArray<int> HexRotator{
		60,
		120,
		180,
		240,
		300,
		360
	};
public:	

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		FRegionData RD;

	UFUNCTION(BlueprintCallable)
	void GenerateChunk();

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		FString OwnerChunk;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		FString ChunkName;


	UPROPERTY(Replicated, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		TArray<UStaticMesh*> ArrayMesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		FRandom_Assets RandomAssets;

	UPROPERTY(BlueprintReadWrite)
		TArray<FArray_unit8> Blocks;

	/////////// Random Assets
	FTimerHandle TimerSetRandomAsset;


	UPROPERTY(BlueprintReadWrite)
		TArray<URandomHISM*> ArrayHISM;


	UFUNCTION(BlueprintCallable)
	void Init_RandomAsset();

	void Check_RandomAsset();

	FRandom_Assets LoadChunkAssetsFromFile(const FString& FilePath);

	bool CheckJsonFileExists(const FString& FilePath);




	void Generate_HISM_RandomAsset();

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		ASpawnRandomManager* ASpawnRandomManager;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		AHexGridManager* Hex;

	UFUNCTION(BlueprintCallable)
		TArray<FTransform> GenerateRandomArray(TArray<int> ContainsArray, const int& iMax, TArray<int>& outArray);


	FTransform GenerateRandom_A_Asset(int& OutIndex);

	UFUNCTION(BlueprintCallable)
		void GenerateRandomAssetsMesh();

	UFUNCTION(BlueprintCallable)
	void Check_ReduceDurability_WithHexIndex(URandomHISM* RHISM, const int& Index, int ReduceValue );


	void Check_ReduceDurability_AllAssets();

	 // Mesh
	void SpawnARandomAssetsMesh(int32 index, const int HexIndex, const FTransform& Transform);

	void SpawnRandomAssetsMesh(int32 index, const TArray<int> HexIndex, const TArray<FTransform>& ArrayTransform);

	UFUNCTION(BlueprintCallable)
		void SaveChunkRandomAssetsToFile(const FString& FilePath, const FRandom_Assets& FChunkAssets);




	void Call_Spawn_Tree(const int HexIndex, const FTransform& t);


	UFUNCTION(NetMulticast, Reliable)
		void MC_Spawn_Tree(const int HexIndex, const  FTransform& t);

	UFUNCTION(NetMulticast, Reliable)
		void MC_Reduce_Durability(AChunkManager* CM, const int& Durability, const int& SlotIndex);

	UFUNCTION(NetMulticast, Reliable)
		void MC_Remove_Tree(const FTransform& Transform, AChunkManager* CM, const int& HexIndex, const int& SlotIndex);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSV_Remove_TreeDelegate, FTransform, Transform, AChunkManager*, CM, int, HexIndex);

	UPROPERTY(BlueprintAssignable, Category = "Triangle")
		FSV_Remove_TreeDelegate SV_Remove_Tree;
	///////////////// Random Assets

	UFUNCTION(BlueprintCallable)
		void AddAChunkOfRegion(const FChunkData& CD , int& ChunkIndex);

	UFUNCTION(BlueprintCallable)
		void AddAChunkOfRegion2(const int& lx, const int& ly, const FArray_unit8& Block, const int& ChunkIndex);

	UFUNCTION(BlueprintCallable)
		void GetBlocksARegion(TArray<uint8>& Block1, TArray<uint8>& Block2);

	UFUNCTION(BlueprintCallable)
		void ConvertBlocksARegion();

	UFUNCTION(BlueprintCallable)
		void CreateArrayHexTransform();

	UFUNCTION(BlueprintCallable)
		void FindTriangleAndRemove(FString Name);

	UFUNCTION(BlueprintCallable)
		void CL_RemoveTriangle(FString Name);

	UFUNCTION(BlueprintCallable)
		void CL_SetBlocksOwnChunkManager(TArray<FArray_unit8> BlocksArray);

	UFUNCTION(BlueprintCallable)
		void CL_GenerateMeshFromBlocks(TArray<FArray_unit8> BlocksArray);

		void SetBlocks(TArray<uint8> Block1, TArray<uint8> Block2);

	UFUNCTION(BlueprintCallable)
		void SaveRegionDataToFile(const FString& FilePath, const FRegionData& RegionData);


	UFUNCTION(BlueprintCallable, Category = "AsyncTask")
		static void AddAChunkOfRegion2Async(int32 lx, int32 ly, FArray_unit8 Block, int ChunkIndex);


	UFUNCTION(BlueprintCallable)
		bool CheckToSpawnTriangle(bool Up, FString TriangleName, TArray<FString> ArrayTriangleName, UHexagonHISM* HISM, int& n, FVector& Location);

	UFUNCTION(BlueprintCallable)
		void SpawnATriangle(int n, FVector Location, FString TriangleName, UHexagonHISM* HISM);

	// Multicast delegate to spawn a triangle
	UPROPERTY(BlueprintAssignable, Category = "Triangle")
		FSpawnTriangleDelegate OnSpawnTriangle;


private: 
	FVector L;
	FRotator R;
	FTransform T;
	TArray<FTransform> DefaultHexTransformdiv2;
	TArray<FTransform> DefaultHexTransform;

	int _X = 0;
	int _Y = 0;

	// Sqrt 1.7
	double s = FMath::Sqrt(3.f) / 2.f;

	// Subtract offset
	double so = ((1 - s) + 1);

	float LX = 0.f;
	float LY = 0.f;

	float sub = so * Range;
	float sub2 = sub / 2.f;
	float ro = Range - Offset;
};
