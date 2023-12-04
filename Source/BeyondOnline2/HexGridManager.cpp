// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/FileHelper.h"
#include "Engine/AssetManager.h"
#include "UObject/SoftObjectPath.h"
#include "Misc/Paths.h"
#include "JsonUtilities.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include <Serialization/BufferArchive.h>
#include "Actors/ChunkManager.h"
#include "Serialization/JsonSerializer.h"
#include <Net/UnrealNetwork.h>


// Sets default values
AHexGridManager::AHexGridManager(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create and attach the root scene component
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // Initialize the first Hierarchical Instanced Static Mesh Component
    HISM1 = CreateDefaultSubobject<UHexagonHISM>(TEXT("HISM1"));
    HISM1->SetupAttachment(GetRootComponent());

    // Initialize the second Hierarchical Instanced Static Mesh Component
    HISM2 = CreateDefaultSubobject<UHexagonHISM>(TEXT("HISM2"));
    HISM2->SetupAttachment(GetRootComponent());
    Noise = new FastNoiseLite();

    if (GIsServer) 
    {
        OwnerRegion = LoadOwnerFromFile();


        //UWorld* World = GetWorld();

        //if (World)
        //{
        //    FActorSpawnParameters SpawnParams;
        //    SpawnParams.Owner = this;
        //    FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
        //    FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

        //    // Spawn the actor
        //    SpawnRandomManager = World->SpawnActor<ASpawnRandomManager>(ASpawnRandomManager::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        //}
    }

}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();
    Noise->SetFrequency(5);
    Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    Noise->SetFractalType(FastNoiseLite::FractalType_FBm);

    //LoadConfigFromFile("ServerConfig.cfg");

}

void AHexGridManager::SaveChunkDataToFile(const FString& FilePath, const FChunkData& ChunkData)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + "/ChunkData/" + FilePath;

    // Convert FChunkData to JSON string
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(ChunkData, JsonString);

    // Save the JSON string to the file
    FFileHelper::SaveStringToFile(JsonString, *AbsoluteFilePath);
}

void AHexGridManager::SaveOwnerToFile(const TMap<FString, FString> Array)
{
    FString Path = FPaths::ProjectDir() + "/RegionData/ChunkManager.json";

    // Convert the TMap to a JSON object
    TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject);
    for (const TPair<FString, FString>& KeyValue : Array)
    {
        Json->SetStringField(KeyValue.Key, KeyValue.Value);
    }

    // Convert the JSON object to a JSON string
    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(Json.ToSharedRef(), JsonWriter);

    // Save the JSON string to the file
    FFileHelper::SaveStringToFile(JsonString, *Path);
}

TMap<FString, FString> AHexGridManager::LoadOwnerFromFile()
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + "/RegionData/ChunkManager.json";

    // Read the JSON data from the file
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *AbsoluteFilePath))
    {
        // Create the file if it doesn't exist
        if (FFileHelper::SaveStringToFile(TEXT(""), *AbsoluteFilePath))
        {
            // The file was created successfully. You can now proceed to work with the empty file.
            UE_LOG(LogTemp, Warning, TEXT("LoadOwnerFromFile: File did not exist, but it was created."));
        }
        return TMap<FString, FString>();
    }

    // Convert the JSON string to a JSON object
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
    {
       // UE_LOG(LogTemp, Error, TEXT("LoadOwnerFromFile: Failed to deserialize JSON data."));
        return TMap<FString, FString>();
    }



    for (const auto& Entry : JsonObject->Values)
    {
        FString Key = Entry.Key;
        FString Value;
        if (Entry.Value->TryGetString(Value))
        {
            OwnerRegion.Add(Key, Value);
            //UE_LOG(LogTemp, Display, TEXT("LoadOwnerFromFile: Key = %s, Value = %s"), *Key, *Value);
        }
    }
    
    return OwnerRegion;
}

void AHexGridManager::GetOwnerMySelf(FString MyName, TArray<FString>& Output_Region)
{
    for (const auto& E : OwnerRegion)
    {
        if (E.Value == MyName)
        {
            Output_Region.Add(E.Key);
            //UE_LOG(LogTemp, Warning, TEXT("Found matching region: Key = %s, Value = %s"), *E.Key, *E.Value);
        }
    }
}


void AHexGridManager::SaveRegionDataToFile(const FString& FilePath, const FRegionData& RegionData)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + "/RegionData/" + FilePath;

    // Convert FChunkData to JSON string
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(RegionData, JsonString);

    // Save the JSON string to the file
    FFileHelper::SaveStringToFile(JsonString, *AbsoluteFilePath);
}

void AHexGridManager::SaveChunkAssetsToFile(const FString& FilePath, const FChunkAssets& FChunkAssets)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + "/ChunkAssets/" + FilePath;

    // Convert FChunkData to JSON string
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(FChunkAssets, JsonString);

    // Save the JSON string to the file
    FFileHelper::SaveStringToFile(JsonString, *AbsoluteFilePath);
}


void AHexGridManager::SaveAChunkAsset(FString Name, FString FileName, FTransform Transform, FAsset_Info Info, FCompountContainer CTag, int& Index)
{
    FChunkAssets CA;
    FChunkAssetsChild CC;
    int z;

    if (CheckJsonFileExists("/ChunkAssets/" + FileName)) // Has file
    {
        CA = LoadChunkAssetsFromFile(FileName);

        z = CA.Name.Find(Name);

        if (z != INDEX_NONE) // if it's valid
        {
            CC.Info.Add(Info);
            CC.Transform.Add(Transform);
            CC.Tags.Add(CTag);
            CA.Assets[z].Transform.Add(Transform);
            CA.Assets[z].Info.Add(Info);
            CA.Assets[z].Tags.Add(CTag);
        }
        else {
            int a = CA.Name.AddUnique(Name);

            if (a != INDEX_NONE)
            {
                CC.Info.Add(Info);
                CC.Transform.Add(Transform);
                CC.Tags.Add(CTag);
                CA.Assets.Add(CC);
            }
        }

    }
    else // Create new
    {
        z = CA.Name.AddUnique(Name);
        
        if (z != INDEX_NONE)
        {
            CC.Info.Add(Info);
            CC.Transform.Add(Transform);
            CC.Tags.Add(CTag);
            CA.Assets.Add(CC);
        }
    }
    Index = z;
    SaveChunkAssetsToFile(FileName, CA);
}

void AHexGridManager::SaveARegionAsset(FString RegionName, FString ObjectName, FTransform Transform, FAsset_Info Info, FCompountContainer CTag, int& Index)
{
    FChunkAssets CA;
    FChunkAssetsChild CC;
    int z;

    if (CheckJsonFileExists("/ChunkAssets/" + RegionName)) // Has file
    {
        CA = LoadChunkAssetsFromFile(RegionName);

        z = CA.Name.Find(ObjectName);

        if (z != INDEX_NONE) // Find Has object
        {
                bool exist = false;
                for (int32 i = 0; i < CA.Assets[z].Info.Num(); ++i)
                {

                    if (CA.Assets[z].Info[i].Name.IsEmpty())
                    { 
                        CA.Assets[z].Transform[i] = Transform;
                        CA.Assets[z].Info[i] = Info;
                        CA.Assets[z].Tags[i] = CTag;
                        z = i;
                        exist = true;
                        break; // Stop the loop once the desired element is found
                    }

                }
                if (!exist) // if the object not exist
                {

                    CC.Info.Add(Info);
                    CC.Transform.Add(Transform);
                    CC.Tags.Add(CTag);
                    CA.Assets[z].Transform.Add(Transform);
                    CA.Assets[z].Info.Add(Info);
                    CA.Assets[z].Tags.Add(CTag);
                    z = CA.Assets[z].Info.Num() - 1;
                }
            

        }
        else {
            int a = CA.Name.AddUnique(ObjectName);

            if (a != INDEX_NONE)
            {
                    CC.Info.Add(Info);
                    CC.Transform.Add(Transform);
                    CC.Tags.Add(CTag);
                CA.Assets.Add(CC);
            }
        }

    }
    else // Create new
    {
        z = CA.Name.AddUnique(ObjectName);

        if (z != INDEX_NONE)
        {
                CC.Info.Add(Info);
                CC.Transform.Add(Transform);
                CC.Tags.Add(CTag);
            CA.Assets.Add(CC);
        }
    }
    Index = z;
    SaveChunkAssetsToFile(RegionName, CA);
}

bool AHexGridManager::CustomizeARegionAsset(FString RegionName, FString NewRegionName, FString ObjectName, FTransform Transform, int Index, int& newIndex, bool& bOtherChunk)
{
    FChunkAssets A;
    int x1 = -1;

    if (CheckJsonFileExists("/ChunkAssets/" + RegionName)) // Has file
    {
        A = LoadChunkAssetsFromFile(RegionName);
        x1 = A.Name.Find(ObjectName);
        //UE_LOG(LogTemp, Warning, TEXT("x1: %d"), x1); // Debug log

        if (x1 != INDEX_NONE) // Find Has object
        {
            if (RegionName == NewRegionName )
            {
                // Check if the index is valid
                if (Index >= 0 && Index < A.Assets[x1].Transform.Num() && !Transform.Equals(A.Assets[x1].Transform[Index]))
                {
                    A.Assets[x1].Transform[Index] = Transform;
                    SaveChunkAssetsToFile(RegionName, A);

                    UE_LOG(LogTemp, Warning, TEXT("RegionName == NewRegionName: Asset updated")); // Debug log
                    return true;
                }
            }
            // RegionName != NewRegionName
            else
            {
                bOtherChunk = true;
                if (CheckJsonFileExists("/ChunkAssets/" + NewRegionName)) // Has file
                {
                    FChunkAssets B = LoadChunkAssetsFromFile(NewRegionName);
                    int x2 = B.Name.Find(ObjectName);
                    int nI = -1;

                    if (x2 != INDEX_NONE) // Find complete
                    {
                        // Find Empty
                        for (int32 i = 0; i < B.Assets[x2].Info.Num(); ++i) {
                            if (B.Assets[x2].Info[i].Name.IsEmpty()) { nI = i; break; }
                        }

                        // Has index
                        if (nI != -1)
                        {
                            if (nI >= 0 && nI < A.Assets[x1].Transform.Num())
                            {
                                SaveARegionAsset(NewRegionName, ObjectName, Transform, A.Assets[x1].Info[Index], A.Assets[x1].Tags[Index], newIndex);
                                RemoveGLTFSlotFromChunkAssets(RegionName, A.Assets[x1].Info[Index].Owner, ObjectName, Index);
                                UE_LOG(LogTemp, Warning, TEXT("Asset moved to another region")); // Debug log
                                return true;
                            }
                        }
                        else {
                            B.Assets[x2].Transform.Add(Transform);
                            B.Assets[x2].Info.Add(A.Assets[x1].Info[Index]);
                            B.Assets[x2].Tags.Add(A.Assets[x1].Tags[Index]);

                            SaveChunkAssetsToFile(RegionName, B);
                            RemoveGLTFSlotFromChunkAssets(RegionName, A.Assets[x1].Info[Index].Owner, ObjectName, Index);

                            newIndex = B.Assets[x2].Info.Num() - 1;
                            //UE_LOG(LogTemp, Warning, TEXT("Asset moved to another region (new)")); // Debug log

                            return true;
                        }
                    } // Not found
                    else {
                        int no = B.Name.AddUnique(ObjectName);

                        if (no != INDEX_NONE)
                        {
                            B.Assets[x2].Transform.Add(Transform);
                            B.Assets[x2].Info.Add(A.Assets[x1].Info[Index]);
                            B.Assets[x2].Tags.Add(A.Assets[x1].Tags[Index]);

                            SaveChunkAssetsToFile(RegionName, B);
                            RemoveGLTFSlotFromChunkAssets(RegionName, A.Assets[x1].Info[Index].Owner, ObjectName, Index);

                            newIndex = B.Assets[x2].Info.Num() - 1;
                            //UE_LOG(LogTemp, Warning, TEXT("Asset moved to another region (new)")); // Debug log

                            return true;
                        }
                    }
                }
                else
                {
                    SaveARegionAsset(NewRegionName, ObjectName, Transform, A.Assets[x1].Info[Index], A.Assets[x1].Tags[Index], newIndex);
                    RemoveGLTFSlotFromChunkAssets(RegionName, A.Assets[x1].Info[Index].Owner, ObjectName, Index);
                    return true;

                    //UE_LOG(LogTemp, Warning, TEXT("NewRegionName")); // Debug log
                }
            }
        }

    }

    newIndex = x1;
    return false;
}

void AHexGridManager::SetMyRegionUnique(TArray<FString> Regions)
{
    for (const FString& Region : Regions)
        MyRegion.AddUnique(Region);

}


FChunkData AHexGridManager::LoadChunkDataFromFile(const FString& FilePath)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + "/ChunkData/" + FilePath;
    FChunkData LoadedChunkData;

    // Load the file as a string
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *AbsoluteFilePath))
    {
        // Handle error if the file couldn't be loaded
        return FChunkData();
    }

    // Convert JSON string to FChunkData
    FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoadedChunkData, 0, 0);

    return LoadedChunkData;
}

FChunkAssets AHexGridManager::LoadChunkAssetsFromFile(const FString& FilePath)
{
    FString Path = FPaths::ProjectDir() + "/ChunkAssets/" + FilePath;
    

    // Load the file as a string
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *Path))
    {
        // Handle error if the file couldn't be loaded
        return FChunkAssets();
    }

    FChunkAssets L;
    // Convert JSON string to FChunkData
    FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &L, 0, 0);

    return L;
}

FRegionData AHexGridManager::LoadRegionDataFromFile(const FString& Name)
{
    FString Path = FPaths::ProjectDir() + "/RegionData/" + Name;

    // Load the file as a string
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *Path))
    {
        // Handle error if the file couldn't be loaded
        return FRegionData();
    }

    FRegionData LoadedRegionData;
    // Convert JSON string to FRegionData
    FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoadedRegionData, 0, 0);

    return LoadedRegionData;
}

bool AHexGridManager::RemoveGLTFSlotFromChunkAssets(const FString& RegionName, const FString& PlayerName, const FString& GLTFName, int32 SlotIndex)
{
    FChunkAssets ChunkAsset = LoadChunkAssetsFromFile(RegionName);

    // Find the chunk asset by name
    int32 ChunkIndex = ChunkAsset.Name.Find(GLTFName);
    if (ChunkIndex != INDEX_NONE)
    {
        FChunkAssetsChild CC = ChunkAsset.Assets[ChunkIndex];

        if (SlotIndex >= 0 && SlotIndex < CC.Transform.Num() && CC.Info[SlotIndex].Owner == PlayerName)
        {
            // Add a debug log to print some information
            //UE_LOG(LogTemp, Warning, TEXT("Found chunk asset: %s"), *GLTFName);
            //UE_LOG(LogTemp, Warning, TEXT("SlotIndex: %d, Owner: %s"), SlotIndex, *PlayerName);

            ChunkAsset.Assets[ChunkIndex].Info[SlotIndex].Name.Empty();

            // Save the updated chunk assets to the file
            SaveChunkAssetsToFile(RegionName, ChunkAsset);

            // Add a debug log to indicate successful update
            //UE_LOG(LogTemp, Warning, TEXT("Chunk asset updated successfully."));
            return true;
        }
    }


    return false;
}

TArray<FChunkData> AHexGridManager::GetChunkDataWithChunkSlot(FVector Location, int number)
{
    TArray<FChunkData> CD;
    FChunkData LoadedChunkData;

    FString FilePath = FPaths::ProjectDir() + "/ChunkData/";

    FString JsonString;



    for (int x = -number; x <= number; x++)
    {
        for (int y = -number; y <= number; y++)
        {
            for (int z = 0; z <= 10; z++)
            {
                FilePath += FString::Printf(TEXT("%i_%i_%i"), x, y, z);

                if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
                    LoadedChunkData.Clear();

                // Convert JSON string to FChunkData
                FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoadedChunkData, 0, 0);

         
                CD.Add(LoadedChunkData);

                //UE_LOG(LogTemp, Warning, TEXT("%s"), CD2.Blocks.IsValidIndex(0));
                //UE_LOG(LogTemp, Warning, TEXT("%i %i %i"), x,y,z);
            }
        }
    }
    return CD;
}

void AHexGridManager::UpdateChunkDataWithAir(const FString& RegionName, int ChunkIndex, const int HISMID, const int BlockIndex)
{
    FString FilePath = "/RegionData/" + RegionName;
    
    // if Region file exist
    if (CheckJsonFileExists(FilePath))
    {
        FRegionData RD = LoadRegionDataFromFile(FilePath);

        // Update the "block"
        if (ChunkIndex >= 0 && ChunkIndex < RD.ChunkData.Num())
        {
            if (HISMID == 1)
                if (BlockIndex >= 0 && BlockIndex < RD.ChunkData[ChunkIndex].Blocks1.Num()) {
                    RD.ChunkData[ChunkIndex].Blocks1[BlockIndex] = 1;
                    //UE_LOG(LogTemp, Warning, TEXT("Debug Log: Updated Block at ChunkIndex: %d, BlockIndex: %d with EBlock::Air"), ChunkIndex, BlockIndex);
                }
            else
                if (BlockIndex >= 0 && BlockIndex < RD.ChunkData[ChunkIndex].Blocks2.Num()) {
                    RD.ChunkData[ChunkIndex].Blocks2[BlockIndex] = 1;
                    //UE_LOG(LogTemp, Warning, TEXT("Debug Log: Updated Block at ChunkIndex: %d, BlockIndex: %d with EBlock::Air"), ChunkIndex, BlockIndex);
                }
            UE_LOG(LogTemp, Warning, TEXT("Debug Log: Updated Block"));

            SaveRegionDataToFile(FilePath, RD);
        }
        else {
            
        }
    }
}

void AHexGridManager::LoadConfigFromFile(FString FilePath, TMap<FString, FString>& ConfigData)
{
    // Get the full path to the config file
    FString FullPath = FPaths::ProjectDir() + FilePath;
    

    // Check if the file exists
    if (!FPaths::FileExists(FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Config file not found: %s"), *FullPath);
    }

    // Read the file into an array of strings
    TArray<FString> FileLines;
    if (!FFileHelper::LoadFileToStringArray(FileLines, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read config file: %s"), *FullPath);
    }

    // Parse the key-value pairs from the file
    for (const FString& Line : FileLines)
    {
        FString Key, Value;
        if (Line.Split(TEXT("="), &Key, &Value))
        {
            ConfigData.Add(Key, Value);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid line in config file: %s"), *Line);
        }
    }
}

void AHexGridManager::FindTriangleInfo(const FString& FilePath, FChunkData& OutChunkData)
{
    OutChunkData = LoadChunkDataFromFile(FilePath);
}

bool AHexGridManager::CheckJsonFileExists(const FString& FilePath)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + FilePath;

    // Check if the file exists
    return FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFilePath);
}

void AHexGridManager::CreateMap(int Size, int Height)
{
    int k = Height / 2;
    int z = 1;
    
    for (int a = -Size; a <= Size; a++)
    {
        for (int b = -Size; b <= Size; b++)
        {
                FString name = FString::Printf(TEXT("%i_%i"), a, b);

                if (!CheckJsonFileExists("/RegionData/" + name))
                {
                    FRegionData RD;
                    RD.RegionName = name;
                    float v1 = a, v2 = b;

                    for (int x = -9 + 1; x <= 9; x++)
                    {
                        for (int y = -9; y <= 9; y++)
                        {
                            FChunkData CD;
                            int cal = FMath::RoundToInt((Noise->GetNoise((v1*10) + x, (v2 * 10) + y) + 1) * k);

                            // z heightmap
                            if (cal > 6) z = 6;
                            else if (cal <= 1) z = 1;
                            else (z = cal);
   
                            CD.x = x;
                            CD.y = y;
                            CD.z = z;

                            // Create height
                            for (int h = 0; h <= z; h++) 
                            {
                                // create a hexagon
                                for (int n = 1; n <= 6; n++)
                                {
                                    FAsset_Info f;
                                    f.Name = FString::Printf(TEXT("%i_%i_%i_%i_a"), x, y, h, n);
                                    f.Durability = 100;
                                    f.NumTris = 200;
                                    f.Owner = "None";
                                    f.Price = 200;
                                    CD.Info1.Add(f);
                                    CD.Blocks1.Add(uint8(EBlock::Dirt));

                                    f.Name = FString::Printf(TEXT("%i_%i_%i_%i_b"), x, y, h, n);
                                    CD.Info2.Add(f);
                                    CD.Blocks2.Add(uint8(EBlock::Dirt));
                                }
                            }
                            if (z < 6) 
                            {
                                // Create height
                                for (int h = z+1; h <= 6; h++)
                                {
                                    // create a hexagon
                                    for (int n = 1; n <= 6; n++)
                                    {
                                        FAsset_Info f;
                                        f.Name = FString::Printf(TEXT("%i_%i_%i_%i_a"), x, y, h, n);
                                        f.Durability = 100;
                                        f.NumTris = 200;
                                        f.Owner = "None";
                                        f.Price = 200;
                                        CD.Info1.Add(f);
                                        CD.Blocks1.Add(uint8(EBlock::Air));

                                        f.Name = FString::Printf(TEXT("%i_%i_%i_%i_b"), x, y, h, n);
                                        CD.Info2.Add(f);
                                        CD.Blocks2.Add(uint8(EBlock::Air));
                                    }
                                }
                            }

                            RD.ChunkData.Add(CD);
                        }
                    }
                    SaveRegionDataToFile(name,RD);
                    RegionsData.Add(RD);
                }    
        }
    }
    
    LoadAllRegion(Size);
}

void AHexGridManager::LoadAllRegion(int Size)
{
    for (int a = -Size; a <= Size; a++)
    {
        for (int b = -Size; b <= Size; b++)
        {

                FString name = FString::Printf(TEXT("%i_%i"), a, b);

                bool bRegionExists = false;
                for (const FRegionData& Region : RegionsData)
                {
                    if (Region.RegionName == name)
                    {
                        bRegionExists = true;
                        break;
                    }
                }

                if (!bRegionExists)
                {
                    // Create and add region data to the RegionsData array
                    RegionsData.Add(LoadRegionDataFromFile(name));
                }
            
        }
    }

    for (auto Key : RegionsData) {
        GenerateRegionChunkMesh(Key);
    }
}

void AHexGridManager::GenerateRegionChunkMesh(FRegionData RegionData)
{
    TArray<FString> NameArray, DirectionArray;
    RegionData.RegionName.ParseIntoArray(NameArray, TEXT("_"), true);

    int32 x = FCString::Atoi(*NameArray[0]);
    int32 y = FCString::Atoi(*NameArray[1]);
    
    FTransform SpawnTransform = FTransform(FVector((x * RangeX), (y * RangeY), 0));
    FString OwnerChunk = "";
    for (auto K : OwnerRegion) {
        if (K.Key == RegionData.RegionName) {
            OwnerChunk = K.Value;
            break;
        }
    }
    OnChunkSpawned.Broadcast(RegionData, SpawnTransform, OwnerChunk);
}


FRegionData AHexGridManager::LoadAndGenerateARegion(const FString& Name)
{
    FRegionData RD;
    // Check Exists
    if (CheckJsonFileExists("/RegionData/" + Name))
    {
        RD = LoadRegionDataFromFile(Name);

        bool bRegionExists = false;
        // Find RegionName from RegionsData
        for (const FRegionData& Region : RegionsData)
        {
            if (Region.RegionName == Name)
            {
                bRegionExists = true;
                break;
            }
        }
        // if not Exists in RegionsData then Load
        if (!bRegionExists)
        {
            RegionsData.Add(RD);
            GenerateRegionChunkMesh(RD);
            return RD;
        }
    }
    return RD;
}

UHexagonHISM* AHexGridManager::FindRegionFromClient(const FString& RegionName, const int& HISMID)
{
    TArray<AActor*> Regions;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChunkManager::StaticClass(), Regions);
    if (Regions.Num() > 0)
    {
        for (auto Chunk : Regions) {
            AChunkManager* CM = Cast<AChunkManager>(Chunk);
            if (CM && CM->ChunkName == RegionName)
                return (HISMID == 1 ? CM->HISM1 : CM->HISM2);
        }
    }
    return nullptr;
}

AChunkManager* AHexGridManager::CL_FindChunkManager(const FString& RegionName)
{
    TArray<AActor*> Regions;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChunkManager::StaticClass(), Regions);
    if (Regions.Num() > 0)
    {
        for (auto Chunk : Regions) {
            AChunkManager* ChunkManager = Cast<AChunkManager>(Chunk);
            if (ChunkManager && ChunkManager->ChunkName == RegionName) {
                return ChunkManager;
            }
                
        }
    }
    return NULL;
}

void AHexGridManager::CL_GenerateRegionMesh(FChunkData ChunkData, FString RegionName)
{
    if (!HISM1 && !HISM2) return;

    //if (Cl_RegionData.RegionName == RegionName) {
    //    ChunkData.
    //}

    TArray<AActor*> Regions;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChunkManager::StaticClass(), Regions);
    if (Regions.Num() > 0)
    {
        int ChunkId = 0;
        for (auto Chunk : Regions)
        {
            AChunkManager* CM = Cast<AChunkManager>(Chunk);
            //bool Has;
            //if (CM && CM->RD.RegionName == RegionName) {
            //    Has = true;
            //    CM->AddAChunkOfRegion(ChunkData, ChunkId);
            //}

            //if (Has == false) 
            //{
            //    FRegionData RegionData;
            //    TArray<FString> NameArray;
            //    RegionName.ParseIntoArray(NameArray, TEXT("_"), true);

            //    int x = FCString::Atoi(*NameArray[0]);
            //    int y = FCString::Atoi(*NameArray[1]);
            //    FTransform SpawnTransform = FTransform(FVector((x * RangeX), (y * RangeY), 0));
            //    RegionData.ChunkData.Add(ChunkData);
            //    OnChunkSpawned.Broadcast(RegionData, SpawnTransform);
            //}
            ChunkId++;
        }
    }
    
}


TArray<FString> AHexGridManager::FindAroundHexagons(FString Name)
{
    TArray<FString> NameArray, DirectionArray;
    Name.ParseIntoArray(NameArray, TEXT("_"), true);

    int32 X = FCString::Atoi(*NameArray[0]);
    int32 Y = FCString::Atoi(*NameArray[1]);

    for (int32 z = 0; z < 6; z++) 
    {
        if (X < 0 && Y < 0) {
            DirectionArray.Add(FString::Printf(TEXT("%i_%i"), X + (Direction[z].X * -1), Y + (Direction[z].Y * -1)));
        }
        else {
            DirectionArray.Add(FString::Printf(TEXT("%i_%i"), X + Direction[z].X, Y + Direction[z].Y));
        }
    }

    return DirectionArray;
}

FRegionData AHexGridManager::FindDataOfRegion(const FString& Name)
{
    for (const FRegionData& Region : RegionsData)
    {
        if (Region.RegionName == Name)
            return Region;
    }
    
    return FRegionData();
}

void AHexGridManager::GetBlocksARegion(FRegionData RD, TArray<FArray_unit8>& B)
{
    int iMax = RD.ChunkData.Num();
    B.SetNum(iMax);
    for (int j = 0; j < iMax; j++)
    {
        int z = RD.ChunkData[j].Blocks1.Num();
        
        for (int c = 0; c < z; c++)
        {
            B[j].Block1.Add(
                (uint8)RD.ChunkData[j].Blocks1[c]
            );

            B[j].Block2.Add(
                (uint8)RD.ChunkData[j].Blocks2[c]
            );
        }
    }
    
}

void AHexGridManager::CL_CreateHexagon(FString Name, FChunkData ChunksData)
{
    if(!HISM1 && !HISM2) return;


    for (int n = 1; n <= 6; n++)
    {

            //if (ChunksData.Blocks[n * 2 - 2] != EBlock::Air)
            //    AddInstanceAsync(HISM1, FTransform(FRotator(0, n * 60, 0), ChunksData.Location[n - 1], FVector(1, 1, 1)), ChunksData.Info[n * 2 - 2].Name, Name, n - 1);

            //if (ChunksData.Blocks[n * 2 - 1] != EBlock::Air)
            //    AddInstanceAsync(HISM2, FTransform(FRotator(0, n * 60, 0), ChunksData.Location[n], FVector(1, 1, 1)), ChunksData.Info[n * 2 - 1].Name, Name, n - 1);
        
    }

    SaveChunkDataToFile(Name, ChunksData);
}

FChunkData AHexGridManager::GetAHexInfo(FString Names)
{
    return LoadChunkDataFromFile(Names);
}

FChunkAssets AHexGridManager::GetHexAssets(FString Name)
{
    return LoadChunkAssetsFromFile(Name);
}


void AHexGridManager::AddInstanceAsync(UHexagonHISM* HISM, const FTransform& Transform, const FString& TriangleName, const FString& Hexagon_Name, const int& Number)
{
    // Create a lambda function to add the instance and store the triangle name
    TFunction<void()> AddInstanceLambda = [=]()
    {
        int32 InstanceIndex = HISM->AddInstance(Transform);
        HISM->Triangle_Names.Add(TriangleName);
        HISM->Hexagon_Name.Add(Hexagon_Name);
        HISM->Number.Add(Number);
        //SetMeshAsync(HISM);

    };


    // Execute the lambda function asynchronously
    FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(AddInstanceLambda, TStatId(), nullptr, ENamedThreads::GameThread);
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
}

void AHexGridManager::SetMeshAsync(UHexagonHISM* HISM)
{
    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
    FString MeshPath = "/Script/Engine.StaticMesh'/Game/Blueprints/Actor_Base/Hexagon/DestructionTri/Tri4_UpTri_Tri4_UpTri.Tri4_UpTri_Tri4_UpTri'";
    FSoftObjectPath MeshSoftPath(MeshPath);

    TSharedPtr<FStreamableHandle> MeshHandle = StreamableManager.RequestAsyncLoad(MeshSoftPath, [HISM, MeshPath]()
        {
            UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);

            if (Mesh)
            {
                HISM->SetStaticMesh(Mesh);

                // Set the material for each instance
                UMaterialInterface* DefaultMaterial = UMaterial::GetDefaultMaterial(MD_Surface);
                if (DefaultMaterial)
                {
                    int32 NumInstances = HISM->GetInstanceCount();
                    for (int32 InstanceIndex = 0; InstanceIndex < NumInstances; ++InstanceIndex)
                    {
                        HISM->SetMaterial(InstanceIndex, DefaultMaterial);
                    }
                }
            }
            else
            {
                // Handle loading failure
            }
        });
}

