// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeyondOnline2.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "HexagonHISM.generated.h"
/**
 * 
 */
UCLASS()
class BEYONDONLINE2_API UHexagonHISM : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
		int ID;

	UPROPERTY(BlueprintReadOnly)
		FString Chunk_Name;

	UPROPERTY(BlueprintReadOnly)
		TArray<FString> Triangle_Names;

	UPROPERTY(BlueprintReadOnly)
		TArray<FVector> Triangle_Location;

	UPROPERTY(BlueprintReadOnly)
		TArray<int> Number;

	UPROPERTY(BlueprintReadOnly)
		TArray<int> ChunkIndex;

	UPROPERTY(BlueprintReadOnly)
		TArray<FString> Hexagon_Name;

	UPROPERTY(BlueprintReadWrite)
		TArray<FAsset_Info> Info;

	UPROPERTY(BlueprintReadWrite)
		TArray<uint8> Blocks;

	UFUNCTION(BlueprintCallable)
		bool SV_Remove_Instance_HISM(int index, FString RegionName, FString PlayerName);


	UFUNCTION(BlueprintCallable)
		void Remove_HISM_Name(FString TriangleName, FTransform& transform);


	UFUNCTION(BlueprintCallable)
		void Remove_HISM_Index(int index, FTransform& transform);

	UFUNCTION(BlueprintCallable)
		void Generate_Triangle(FTransform transform, FString TriangleName, int& Index);
};
