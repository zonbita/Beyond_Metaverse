// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/SpawnRandomManager.h"
#include <Net/UnrealNetwork.h>
#include "../BeyondOnline2.h"
#include "./ChunkManager.h"
#include "../HexGridManager.h"
#include <JsonUtilities.h>

// Sets default values
ASpawnRandomManager::ASpawnRandomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    // Create and attach the root scene component
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // Set this actor to replicate
    bReplicates = true;
}

// Called when the game starts or when spawned
void ASpawnRandomManager::BeginPlay()
{
	Super::BeginPlay();


    if (GIsServer)
    {
        GetWorldTimerManager().SetTimer(TimerCounter, this, &ASpawnRandomManager::AddARandomAsset, 100.0f, true);
        GetWorldTimerManager().SetTimer(TimerReduceDurability, this, &ASpawnRandomManager::ReduceTreeDurability, 3600.0f, true);
    }
}

void ASpawnRandomManager::ReduceTreeDurability()
{
    TArray<AActor*> OutActors;
    Hex->GetAttachedActors(OutActors, true, false);

    // Get All Attached CM
    for (auto O : OutActors)
    {
        AChunkManager* CM = Cast<AChunkManager>(O);
        if (CM) 
        {
            CM->Check_ReduceDurability_AllAssets();
        }
    }
}


void ASpawnRandomManager::AddARandomAsset()
{
    if (!GIsServer) return;

    TArray<AActor*> OutActors;

    if (!Hex) return;

     Hex->GetAttachedActors(OutActors, true, false);

    if (OutActors.IsEmpty())  return;
    
    AChunkManager* CM;

    // Get ChunkManager Childs of HexGridManager 
    for (auto O : OutActors)
    {
        CM = Cast<AChunkManager>(O);
        if (CM)
        {
            for (int i = 0; i < CM->ArrayHISM.Num(); ++i)
            {
                    TArray<int> New;

                    // Count Tree Totals 
                    for (auto A : CM->RandomAssets.Asset)
                        New.Append(A.Indexs);

                    if (New.Num() < 20)
                    {
                        int j;
                        FTransform t = CM->GenerateRandom_A_Asset(j);
                        break;
                    } 
            }
        }
    }
    

}


void ASpawnRandomManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME conditionally replicates the property if it meets the conditions (e.g., if it's marked for Replication).
    DOREPLIFETIME(ASpawnRandomManager, Hex);
    
}