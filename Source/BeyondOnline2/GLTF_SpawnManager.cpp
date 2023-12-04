// Fill out your copyright notice in the Description page of Project Settings.


#include "GLTF_SpawnManager.h"
#include <JsonUtilities.h>
#include <JsonObjectConverter.h>
#include "HexGridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AC_Trace.h"
#include "Components/AC_NFT.h"
#include "BeyondOnline2Character.h"


// Sets default values
AGLTF_SpawnManager::AGLTF_SpawnManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGLTF_SpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	if(GIsServer)
		GetWorldTimerManager().SetTimer(TimerReduceDurability, this, &AGLTF_SpawnManager::Check_ReduceDurability_AllAssets, 6000.0f, true);
}

FChunkAssets AGLTF_SpawnManager::LoadChunkAssetsFromFile(const FString& FilePath)
{
	FString Path = FPaths::ProjectDir() + "/ChunkAssets/" + FilePath;
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *Path))
		return FChunkAssets();

	FChunkAssets L;

	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &L, 0, 0);

	return L;
}

void AGLTF_SpawnManager::Get_Mouse_Location(FVector& Location, ECollisionChannel TraceChannel)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
		return;

	FVector2D MousePosition;
	float ViewportScale;

	// Get the mouse position on the viewport
	MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	// Get the viewport scale
	ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	MousePosition = MousePosition * ViewportScale;

	FVector3d oLocation, Direction;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, oLocation, Direction);

	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, oLocation, Direction * 200000 + oLocation, TraceChannel);
	if (bHit) {
		Location = oLocation;
	}
	else return;
}

FChunkAssets AGLTF_SpawnManager::SV_GetAssetsARegion_SendToClient(FString RegionName)
{
	if (SV_Region_Assets.Contains(RegionName)) {
		FChunkAssets* CA = SV_Region_Assets.Find(RegionName);
		return *CA;
	}
	else {
		return LoadChunkAssetsFromFile(RegionName);
	}

}

void AGLTF_SpawnManager::SV_SetAssetsARegion(FString RegionName, FChunkAssets Assets)
{
	if (SV_Region_Assets.Contains(RegionName)) {
		SV_Region_Assets[RegionName] = Assets;
	}
}

void AGLTF_SpawnManager::GetAssetNotEmpty_SendToClient(const FChunkAssetsChild& Assets, TArray<int>& OutSlot)
{
	for (int i = 0; i < Assets.Info.Num(); i++)
		if (!Assets.Info[i].Name.IsEmpty())
			OutSlot.Add(i);
} 

void AGLTF_SpawnManager::GetAssetsNotEmpty(const FChunkAssets& Assets, FChunkAssets& outAssets)
{

	for (int i = 0; i < Assets.Name.Num(); i++) {
		for (int j = 0; j < Assets.Assets[i].Info.Num(); j++) {
			if (!Assets.Assets[i].Info[j].Name.IsEmpty())
				Assets.Assets[i].Info[j].StaticStruct();
		}
	}
	outAssets = Assets;
}


bool AGLTF_SpawnManager::SetMaterialColorGLTF(const FString& RegionName, const FString& GLTFName, const TArray<FColorMaterials>& ColorMaterials, int32 SlotIndex, FCompountContainer& Data)
{
	FChunkAssets CA = LoadChunkAssetsFromFile(RegionName);

	int z = CA.Name.Find(GLTFName);

	if (z != INDEX_NONE && CA.Assets.IsValidIndex(z)) // Check valid index
	{
		if (CA.Assets[z].Tags.IsValidIndex(SlotIndex))  // SlotIndex is valid
		{
			FCompountContainer& FCC = CA.Assets[z].Tags[SlotIndex];

			for (int j = 0; j < ColorMaterials.Num(); j++)
			{
				
				bool b = false;
				// loop for Tags array
				for (int32 Index = 0; Index < FCC.Data.Num(); ++Index)
				{
					if (FCC.Data[Index].Name == ColorMaterials[j].ComponnentName)
					{
						FCC.Data[Index].TagValue.Add("TableIndex", FString::FromInt(ColorMaterials[j].TableIndex));
						FCC.Data[Index].TagValue.Add("RowName", ColorMaterials[j].RowName.ToString());
						FCC.Data[Index].TagValue.Add("Color", ColorMaterials[j].Color.ToString());

						b = true;

						break;
					}
				}
				if (!b)
				{
					FCompountTags CC;
					CC.Name = ColorMaterials[j].ComponnentName;
					CC.TagValue.Add("TableIndex", FString::FromInt(ColorMaterials[j].TableIndex));
					CC.TagValue.Add("RowName", ColorMaterials[j].RowName.ToString());
					CC.TagValue.Add("Color", ColorMaterials[j].Color.ToString());


					FCC.Data.Add(CC);
					Data = FCC;
				}

			}


			SaveChunkAssetsToFile(RegionName, CA);
			return true;
		}
	}

	return false;
}

void AGLTF_SpawnManager::SaveChunkAssetsToFile(const FString& FilePath, const FChunkAssets& FChunkAssets)
{
	FString AbsoluteFilePath = FPaths::ProjectDir() + "/ChunkAssets/" + FilePath;

	// Convert FChunkData to JSON string
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FChunkAssets, JsonString);

	// Save the JSON string to the file
	FFileHelper::SaveStringToFile(JsonString, *AbsoluteFilePath);
}

void AGLTF_SpawnManager::Check_ReduceDurability_WithIndex(AActor* actor, const FString& RegionName, const FString& GLTFName, const int& Index, int ReduceValue)
{
	for (auto& P : SV_Region_Assets)
	{
		FString Key = P.Key;
		FChunkAssets Value = P.Value;
		int i = Key.Find(RegionName);


		if (i != INDEX_NONE && P.Value.Assets.IsValidIndex(i))
		{
			if (P.Value.Assets[i].Info.IsValidIndex(Index))
			{
				P.Value.Assets[i].Info[Index].Durability -= ReduceValue;

				if (P.Value.Assets[i].Info[Index].Durability <= 0.0)
				{
					P.Value.Assets[i].Info[Index].Durability = 0;
					P.Value.Assets[i].Info[Index].Name = "";
					MC_Reduce_Durability(actor, RegionName, GLTFName, 0);
					
				}
				else
				{
					MC_Reduce_Durability(actor, RegionName, GLTFName, P.Value.Assets[i].Info[Index].Durability);
			
				}

				SaveChunkAssetsToFile(RegionName, P.Value);
				break;
			}
		}
	}
}

void AGLTF_SpawnManager::Check_IncreaseDurability_WithIndex(AActor* actor, const FString& RegionName, const FString& GLTFName, const int& Index, int IncreaseValue)
{
	for (auto& P : SV_Region_Assets)
	{
		FString Key = P.Key;
		FChunkAssets Value = P.Value;
		int i = Key.Find(RegionName);

		if (i != INDEX_NONE && P.Value.Assets.IsValidIndex(i))
		{
			if (P.Value.Assets[i].Info.IsValidIndex(Index))
			{
				P.Value.Assets[i].Info[Index].Durability += IncreaseValue;

				if (P.Value.Assets[i].Info[Index].Durability >= 100.0)
				{
					P.Value.Assets[i].Info[Index].Durability = 100;
					MC_Reduce_Durability(actor, RegionName, GLTFName, 100);
				}
				else MC_Reduce_Durability(actor, RegionName, GLTFName, P.Value.Assets[i].Info[Index].Durability);

				SaveChunkAssetsToFile(RegionName, P.Value);
				break;
			}
		}
	}
}

int FindIndexForKey(const TArray<FString> Keys, const FString& KeyToFind)
{
	int Index = Keys.Find(KeyToFind);

	if (Index != -1) return Index;
	else return -1;
}

void AGLTF_SpawnManager::Check_ReduceDurability_AllAssets()
{
	if (!GIsServer) return;

	for (auto& P : SV_Region_Assets)
	{
		FString Key = P.Key;
		FChunkAssets Value = P.Value;

		for (int j = 0; j < Value.Assets.Num(); j++)
		{
			for (int i = 0; i < Value.Assets[j].Info.Num(); i++)
			{
				if (!P.Value.Assets[j].Info[i].Name.IsEmpty()) 
				{
					if (Value.Assets[j].Info[i].Durability > 0)
					{
						P.Value.Assets[j].Info[i].Durability -= 1;
					}
					else {
						P.Value.Assets[j].Info[i].Durability = 0;
						P.Value.Assets[j].Info[i].Name = "";
					}
				}

					
				
			}
				
		}
		SaveChunkAssetsToFile(P.Key, P.Value);
	}

}

void AGLTF_SpawnManager::MC_Reduce_Durability_Implementation(AActor* actor, const FString& RegionName, const FString& GLTFName, const float& Durability)
{
	if (actor)
	{
		if (Durability <= 0) 
		{
			TArray<AActor*> A;
			actor->GetAttachedActors(A, true, false);

			if (!A.IsEmpty())
			{
				for (auto a : A)
				{
					if (a) a->Destroy();
				}

			}
			actor->Destroy();

		}
		else {
			
			if (GIsClient)
			{
				TArray<AActor*> A;
				actor->GetAttachedActors(A, true, false);

				if (!A.IsEmpty())
				{
					for (auto a : A) 
					{
						UAC_NFT* NFT = a->FindComponentByClass<UAC_NFT>();
						if(NFT) NFT->Asset_Info.Durability = Durability;
					}
					
				}

			}
		}

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 0 represents the local player index
		if (PC)
		{
			APawn* BOC = Cast<APawn>(PC->GetPawn());
			if (BOC)
			{
				UAC_Trace* AT = BOC->FindComponentByClass<UAC_Trace>();
				if (AT)
				{
					if (AT->Refesh_WidgetInfo.IsBound())
					{
						AT->Refesh_WidgetInfo.Execute();
					}
				}
			}
		}
	} 
}