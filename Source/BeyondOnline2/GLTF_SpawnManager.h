// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "../../Plugins/glTFRuntime/Source/glTFRuntime/Public/glTFRuntimeAsset.h"

#include "BeyondOnline2.h"
#include "Async_MakeGETAPI2.h"
#include <glTFRuntimeAssetActorAsync.h>
#include "GLTF_SpawnManager.generated.h"




UCLASS()
class BEYONDONLINE2_API AGLTF_SpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGLTF_SpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	

	UFUNCTION(BlueprintCallable)
	FChunkAssets LoadChunkAssetsFromFile(const FString& FilePath);

	UFUNCTION(BlueprintCallable)
		void Get_Mouse_Location(FVector& Location, ECollisionChannel TraceChannel);


	UFUNCTION(BlueprintCallable)
		FChunkAssets SV_GetAssetsARegion_SendToClient(FString RegionName);


	UFUNCTION(BlueprintCallable)
		void SV_SetAssetsARegion(FString RegionName, FChunkAssets Assets);

	UFUNCTION(BlueprintCallable)
		static void GetAssetNotEmpty_SendToClient(const FChunkAssetsChild& Assets, TArray<int>& OutSlot);

	// Set Material and GLTF Asset Color
	UFUNCTION(BlueprintCallable)
		bool SetMaterialColorGLTF(const FString& RegionName, const FString& GLTFName, const TArray<FColorMaterials>& ColorMaterials, int32 SlotIndex, FCompountContainer& Data);

	UFUNCTION(BlueprintCallable)
		void SaveChunkAssetsToFile(const FString& FilePath, const FChunkAssets& FChunkAssets);

	/// <summary>
	///  Durability
	/// </summary>
	int FindIndexForKey(const TArray<FString> Keys,const FString& KeyToFind);


	UFUNCTION(BlueprintCallable)
		void Check_ReduceDurability_WithIndex(AActor* actor, const FString& RegionName, const FString& GLTFName,const int& Index, int ReduceValue);

	UFUNCTION(BlueprintCallable)
		void Check_IncreaseDurability_WithIndex(AActor* actor, const FString& RegionName, const FString& GLTFName, const int& Index, int IncreaseValue);

		void Check_ReduceDurability_AllAssets();

	UFUNCTION(NetMulticast, Reliable)
		void MC_Reduce_Durability(AActor* actor, const FString& RegionName, const FString& GLTFName, const float& Durability);

	//UFUNCTION(NetMulticast, Reliable)
		//void MC_Remove_GLTF(const FString& GLTFName, const FTransform& Transform, const int& HexIndex, const int& SlotIndex);

	/// <summary>
	///  Durability
	/// </summary>
	/// 
	UFUNCTION(BlueprintCallable)
		static void GetAssetsNotEmpty(const FChunkAssets& Assets, FChunkAssets& outAssets);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
		FString Url;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
		TMap<FString, UglTFRuntimeAsset*> GLTF_List;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
		int32 Length;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
		TObjectPtr<UglTFRuntimeAsset> Asset;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
		TMap<FString, FChunkAssets> CL_GLTF_Assets;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
		TMap<FString, FChunkAssets> SV_Region_Assets;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
		bool Load_Complete = true;

	FTimerHandle TimerReduceDurability;
};
