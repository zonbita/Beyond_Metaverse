// Fill out your copyright notice in the Description page of Project Settings.
#include "B_PlayerController.h"
#include "Kismet/GameplayStatics.h"


FIntVector2 AB_PlayerController::CalculatorRegionforOwnClient()
{
	APawn* ControlledPawn = GetPawn();
	FVector PlayerLocation;
	if (ControlledPawn)
		PlayerLocation = ControlledPawn->GetActorLocation();

	FIntVector2 v;
	if (PlayerLocation.X < RangeX2 && PlayerLocation.X > -RangeX2)
		v.X = 0 ;
	else
		v.X = FMath::RoundToInt(PlayerLocation.X / RangeX);


	if (PlayerLocation.Y < RangeY2 && PlayerLocation.Y > -RangeY2)
		v.Y = 0;
	else
		v.Y = FMath::RoundToInt(PlayerLocation.Y / RangeY);

	return v;
}

TArray<FString> AB_PlayerController::GetRegionsAroundPlayer()
{
	FIntVector2 XY = CalculatorRegionforOwnClient();
	TArray<FString> DirectionArray;

	// Add mid region
	DirectionArray.Add(FString::Printf(TEXT("%i_%i"), XY.X, XY.Y));

	for(auto Vector : Direction2D)
	{
		if (XY.X < 0 && XY.Y < 0)
			DirectionArray.Add(FString::Printf(TEXT("%i_%i"), XY.X + (Vector.X * -1), XY.Y + (Vector.Y * -1)));
		else 
			DirectionArray.Add(FString::Printf(TEXT("%i_%i"), XY.X + Vector.X, XY.Y + Vector.Y));
		
	}
	return DirectionArray;
}

void AB_PlayerController::GetRegionPlayer(int& x, int& y)
{
    FIntVector2 XY = CalculatorRegionforOwnClient();
    x = XY.X;
    y = XY.Y;
}
TArray<FRegionData> AB_PlayerController::RegionsAroundPlayerNeedSpawn(TArray<FString> Regions)
{
    TArray<FRegionData> New;

    if (!Regions.IsEmpty())
    {
        if (HexGridManager)
        {

            for (FString Name : Regions)
            {
                if (!RegionsName.Contains(Name))
                {

                    FRegionData FoundRegionData;

                    // Find RegionName from RegionsData
                    for (const FRegionData& Region : HexGridManager->RegionsData)
                    {
                        if (Region.RegionName == Name)
                        {
                            FoundRegionData = Region;
                            break;
                        }
                    }

                    if (FoundRegionData.RegionName.IsEmpty())
                    {
                        FRegionData RD = HexGridManager->LoadAndGenerateARegion(Name);
                        New.Add(RD);
                    }
                    else
                        New.Add(FoundRegionData);
                    
                    RegionsName.Add(Name);
                }
            }
        }
    }
    return New;
}

void AB_PlayerController::SetOwnerChunkManager(APawn* NewOwnerPawn, FString PlayerName)
{
        TArray<AActor*> Actors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChunkManager::StaticClass(), Actors);

        for (auto Ac : Actors) 
        {
            AChunkManager* A = Cast<AChunkManager>(Ac);
            if (A) 
                if (A->OwnerChunk == PlayerName)
                    A->SetOwner(NewOwnerPawn);
        }
}

/// <summary>
/// If PlayerName === Owner Chunk
/// </summary>
/// <param name="Location"></param>
/// <param name="TraceChannel"></param>
/// <param name="PlayerName"></param>
/// <param name="RegionName"></param>
/// <param name="OwnerName"></param>
/// <param name="TriangleName"></param>
/// <returns></returns>
bool AB_PlayerController::TraceChunk(const FVector& Location, ETraceTypeQuery TraceChannel, FString PlayerName, FString& RegionName, FString& OwnerName, FString& TriangleName)
{
    TArray<AActor*> ignoreActors;
    FHitResult Hit;
    bool b = UKismetSystemLibrary::BoxTraceSingle(this, Location,Location, FVector(100,100, 10000.0), FRotator(0,0,0),
        TraceChannel, false, ignoreActors, EDrawDebugTrace::None, Hit, true);

    if (b)
    {
        AChunkManager* ChunkManager = Cast<AChunkManager>(Hit.GetActor());
       

        if (PlayerName == ChunkManager->OwnerChunk && ChunkManager) 
        {
            RegionName = ChunkManager->RD.RegionName;
            OwnerName = PlayerName;
            UHexagonHISM* HH = Cast<UHexagonHISM>(Hit.GetComponent());
            if(HH)
                TriangleName = HH->Triangle_Names[Hit.Item];
            return true;
        }
    }
    return false;
}
