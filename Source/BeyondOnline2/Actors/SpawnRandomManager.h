// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include <BeyondOnline2/BeyondOnline2.h>
#include "../Actors/RandomHISM.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "SpawnRandomManager.generated.h"


class AChunkManager;
class AHexGridManager;



UCLASS()
class BEYONDONLINE2_API ASpawnRandomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnRandomManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		AHexGridManager* Hex;

	FTimerHandle TimerCounter, TimerReduceDurability;

	void ReduceTreeDurability();

	void AddARandomAsset();

};
