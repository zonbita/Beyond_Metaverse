// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <BeyondOnline2/BeyondOnline2.h>
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "RandomHISM.generated.h"

class AChunkManager;
/**
 * 
 */
UCLASS()
class BEYONDONLINE2_API URandomHISM : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	AChunkManager* ChunkManager;

	UPROPERTY(BlueprintReadWrite)
	TArray<int> Hex_Index;
};
