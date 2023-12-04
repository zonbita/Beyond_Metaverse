// Fill out your copyright notice in the Description page of Project Settings.

#include "HexagonHISM.h"
#include "HexGridManager.h"
#include "./Actors/ChunkManager.h"
#include "LoggerBPLibrary.h"
#include <Net/UnrealNetwork.h>


// Record remove log, Remove Instance in server
bool UHexagonHISM::SV_Remove_Instance_HISM(int index, FString RegionName, FString PlayerName)
{
    if (IsValidInstance(index))
    {
        FString n, tn;
        if (index >= 0 && index < Triangle_Names.Num()) 
        {
            tn = Triangle_Names[index];
            n = FString::Printf(TEXT("Region: %s | Triangle Name: %s "), *RegionName, *tn);
        }
        else
            n = FString::Printf(TEXT("Invalid index: %d"), index);

        AChunkManager* CM = Cast<AChunkManager>(GetOwner());

        if (CM) 
        {
            //UE_LOG(LogTemp, Warning, TEXT("Successfully casted owner to AChunkManager"));

            CM->FindTriangleAndRemove(tn);

            CM->SaveRegionDataToFile(CM->RD.RegionName, CM->RD);

            //UE_LOG(LogTemp, Warning, TEXT("Saving region data for region: %s"), *CM->RD.RegionName);
        }


        FDateTime D = FDateTime::Now();
        FString Text = FString::Printf(TEXT("%02d:%02d:%02d | PlayerName: %s / %s "), D.GetHour(), D.GetMinute(), D.GetSecond(), *PlayerName, *n);
        ULoggerBPLibrary::WriteLn(Text);


        RemoveInstance(index);
    }
    return false;
}
/// Remove from clients
void UHexagonHISM::Remove_HISM_Name(FString TriangleName, FTransform &transform)
{
    int Index = Triangle_Names.Find(TriangleName);
    if (Index != INDEX_NONE)
    {
        this->GetInstanceTransform(Index, transform);
        RemoveInstance(Index);

        UE_LOG(LogTemp, Warning, TEXT("Triangle with name %s "), *TriangleName);
    }
}

void UHexagonHISM::Remove_HISM_Index(int index, FTransform& transform)
{
    int i = GetInstanceTransform(index, transform);
    if(i != INDEX_NONE)
        RemoveInstance(index);
}

void UHexagonHISM::Generate_Triangle(FTransform transform, FString TriangleName, int& Index)
{
    int i = AddInstance(transform , true);
    if (!Triangle_Names.IsEmpty() && Triangle_Names.IsValidIndex(i))
        Triangle_Names[i].InsertAt(i, TriangleName);
    else
        Triangle_Names.Add(TriangleName);

    Index = i;

    if (GIsServer && GetOwner()) {
        AChunkManager* CM = Cast<AChunkManager>(GetOwner());
    }
}

//void UHexagonHISM::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
//{
//    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//    // DOREPLIFETIME conditionally replicates the property if it meets the conditions (e.g., if it's marked for Replication).
//}