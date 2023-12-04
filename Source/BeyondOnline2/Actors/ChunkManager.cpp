// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/ChunkManager.h"
#include <JsonUtilities.h>
#include "Async/Async.h"
#include <Net/UnrealNetwork.h>
#include <Kismet/GameplayStatics.h>
#include <BeyondOnline2/HexGridManager.h>
#include <BeyondOnline2/B_PlayerController.h>
#include <BeyondOnline2/BeyondOnline2Character.h>

// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create and attach the root scene component
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // Initialize the first Hierarchical Instanced Static Mesh Component
    HISM1 = CreateDefaultSubobject<UHexagonHISM>(TEXT("HISM1"));
    HISM1->SetupAttachment(GetRootComponent());
    HISM1->ID = 1;

    // Initialize the second Hierarchical Instanced Static Mesh Component
    HISM2 = CreateDefaultSubobject<UHexagonHISM>(TEXT("HISM2"));
    HISM2->SetupAttachment(GetRootComponent());
    HISM2->ID = 2;

    // Set this actor to replicate
    bReplicates = true;


}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay()
{
	Super::BeginPlay();
    //if (GIsServer) {
    //    GenerateChunk();
    //}
    TArray<FString> NameArray;
    ChunkName.ParseIntoArray(NameArray, TEXT("_"), true);

    if (!NameArray.IsEmpty()) 
    {
        int _X = FCString::Atoi(*NameArray[0]);
        int _Y = FCString::Atoi(*NameArray[1]);
    }

    GetWorldTimerManager().SetTimer(TimerSetRandomAsset, this, &AChunkManager::Check_RandomAsset, 0.1f, true, 2.0);
}

void AChunkManager::GenerateChunk()
{
    if (!HISM1 || !HISM2) return;

    if (RD.ChunkData.IsValidIndex(0)) 
    {
        for (int j = 0; j < RD.ChunkData.Num(); j++)
            if (RD.ChunkData.IsValidIndex(j)) {
                AddAChunkOfRegion(RD.ChunkData[j], j);
                Init_RandomAsset();
                
            }
                

        //ConvertBlocksARegion();
            
    }
}

void AChunkManager::AddAChunkOfRegion(const FChunkData& CD, int& ChunkIndex)
{
    LX = (CD.x % 2 == 0) ? (sub2 + (CD.y * sub)) : (CD.y * sub);
    LY = CD.x * ro;

    int Num = 0;
    for (int z = 0; z <= 6; z++)
    {
        L = FVector(LX, LY, (z * Height));
        for (int n = 1; n <= 6; n++)
        {
            int index = 0, n2 = n-1;

            if(z==0)
                index = z+(n2);
            else
                index = (z*6)+ (n2);


            if (CD.Blocks1.IsValidIndex(index)) {
                if (CD.Blocks1[index] != 1) {
                    AddInstanceAsync(HISM1, FTransform(FRotator(0, HexRotator[n2], 0), L, FVector(1, 1, 1)),
                        CD.Info1[index].Name,
                        CD.Info1[index].Name,
                        ChunkIndex,
                        Num,
                        CD.Blocks1[Num],
                        CD.Info1[index]);
                }
            }
               
            if (CD.Blocks2.IsValidIndex(index)) {
                if(CD.Blocks2[index] != 1)
                AddInstanceAsync(HISM2, FTransform(FRotator(0, HexRotator[n2], 0), L, FVector(1, 1, 1)),
                    CD.Info2[index].Name,
                    CD.Info2[index].Name,
                    ChunkIndex, Num, CD.Blocks2[Num],
                    CD.Info2[index]);

            }
             
            Num++;
            
        }
    }

    if(GIsServer)  CompletedHex = true;
    //UE_LOG(LogTemp, Warning, TEXT("Num after increment: %d"), Num);
}

void AChunkManager::AddAChunkOfRegion2(const int& lx, const int& ly, const FArray_unit8& Block, const int& ChunkIndex)
{
    LX = (lx % 2 == 0) ? (sub2 + (ly * sub)) : (ly * sub);
    LY = lx * ro;

    int Num = 0;
    FAsset_Info f;
    for (int z = 0; z <= 6; z++)
    {
        L = FVector(LX, LY, (z * Height));
        FString namePrefix = FString::Printf(TEXT("%i_%i_%i_"), lx, ly, z);
        for (int n = 1; n <= 6; n++)
        {
            FString nameSuffix = FString::Printf(TEXT("%i_a"), n);
            f.Name = namePrefix + nameSuffix;

            f.Durability = 100;
            f.NumTris = 200;
            f.Owner = "None";
            f.Price = 200;

            if (Block.Block1[Num] != 1)
                AddInstanceAsync(HISM1, FTransform(FRotator(0, HexRotator[n - 1], 0), L, FVector(1, 1, 1)), f.Name, f.Name, ChunkIndex, Num, Num, f);

            nameSuffix = FString::Printf(TEXT("%i_b"), n);
            f.Name = namePrefix + nameSuffix;

            if (Block.Block2[Num] != 1)
                AddInstanceAsync(HISM2, FTransform(FRotator(0, HexRotator[n - 1], 0), L, FVector(1, 1, 1)), f.Name, f.Name, ChunkIndex, Num, Num, f);

            Num++;

        }
    }
}

void AChunkManager::ConvertBlocksARegion()
{
    int max = RD.ChunkData.Num();
    Blocks.SetNum(max);
    //(LogTemp, Warning, TEXT("Processing Chunk %d"), RD.ChunkData.Num());

    for (int j = 0; j < max; j++)
    {
        if (RD.ChunkData.IsValidIndex(j))
        {
            for (int c = 0; c < 42; c++)
            {
                if (RD.ChunkData[j].Blocks1.IsValidIndex(c) && RD.ChunkData[j].Blocks2.IsValidIndex(c))
                {
                    Blocks[j].Block1.Add(
                        (uint8)RD.ChunkData[j].Blocks1[c]
                    );

                    Blocks[j].Block2.Add(
                        (uint8)RD.ChunkData[j].Blocks2[c]
                    );

                   // UE_LOG(LogTemp, Warning, TEXT("  Processing Block %d in Chunk %d"), c, j);
                }
                //else
                //{
                //    UE_LOG(LogTemp, Error, TEXT("Invalid Block Index in Chunk %d, Block1 size: %d, Block2 size: %d"), j, RD.ChunkData[j].Blocks1.Num(), RD.ChunkData[j].Blocks2.Num());
                //}
            }
        }
    }
}

void AChunkManager::CreateArrayHexTransform()
{
    LX = 0.f;
    LY = 0.f;
    LX = LX +  sub;
    LY = ro;
    FChunkInstance CI;
    int Num = 0;
    for (int z = 0; z <= 6; z++)
    {
        L = FVector(LX, LY, (z * Height));
        for (int n = 1; n <= 6; n++)
        {
            Num++;
            
        }
    }
}

void AChunkManager::SaveRegionDataToFile(const FString& FilePath, const FRegionData& RegionData)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + "/RegionData/" + FilePath;

    // Convert FChunkData to JSON string
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(RegionData, JsonString);

    // Save the JSON string to the file
    FFileHelper::SaveStringToFile(JsonString, *AbsoluteFilePath);
}


void AChunkManager::GetBlocksARegion(TArray<uint8>& Block1, TArray<uint8>& Block2)
{

    for (auto c : RD.ChunkData) {
        int NumBlocks = c.Blocks1.Num();
        for (int z = 0; z < NumBlocks; z++) {
            if(z <= c.Blocks1.Num())
                Block1.Add((uint8)c.Blocks1[z]);
            if (z <= c.Blocks2.Num())
                Block2.Add((uint8)c.Blocks2[z]);
        }
    }
}



void AChunkManager::FindTriangleAndRemove(FString Name)
{
        int iMax = RD.ChunkData.Num();

        for (int k = 0; k < iMax; k++)
        {
            if (Name.Contains("a"))
            {
                int max = RD.ChunkData[k].Info1.Num();
                for (int z = 0; z < max; z++)
                    if (RD.ChunkData[k].Info1[z].Name == Name)
                    {
                        RD.ChunkData[k].Blocks1[z] = 1;
                   
                    }
            }
            else {
                int max = RD.ChunkData[k].Info2.Num();

                for (int z = 0; z < max; z++)
                    if (RD.ChunkData[k].Info2[z].Name == Name)
                        RD.ChunkData[k].Blocks2[z] = 1;
            }

        }
}

void AChunkManager::CL_RemoveTriangle(FString Name)
{

        if (Name.Contains("a"))
        {
            int x = HISM1->Triangle_Names.Num();

            for (int z = 0; z < x; z++)
                if (HISM1->Triangle_Names[z] == Name)
                    HISM1->RemoveInstance(z);
        }
        else {
            int x = HISM2->Triangle_Names.Num();

            for (int z = 0; z < x; z++)
                if (HISM2->Triangle_Names[z] == Name)
                    HISM2->RemoveInstance(z);
        }

    
}

void AChunkManager::CL_SetBlocksOwnChunkManager(TArray<FArray_unit8> BlocksArray)
{
    int iMax = BlocksArray.Num();

    for (int j = 0; j < iMax; j++)
    {
        int zMax = BlocksArray[j].Block1.Num();

        for (int z = 0; z < zMax; z++)
        {
                if (BlocksArray[j].Block1.IsValidIndex(z) && BlocksArray[j].Block1[z] == 1)
                    HISM1->RemoveInstance(z);

                if (BlocksArray[j].Block2.IsValidIndex(z) && BlocksArray[j].Block2[z] == 1)
                    HISM2->RemoveInstance(z);
        }
    }
}

void AChunkManager::CL_GenerateMeshFromBlocks(TArray<FArray_unit8> BlocksArray)
{
    int j = 0;


    for (int x = -9 + 1; x <= 9; x++)
    {
        for (int y = -9; y <= 9; y++)
        {
            AddAChunkOfRegion2(x, y, BlocksArray[j],j);
            //AddAChunkOfRegion2Async(x, y, BlocksArray[j], j);
            j++;
        }

    }
    CompletedHex = true;
}

void AChunkManager::SetBlocks(TArray<uint8> Block1, TArray<uint8> Block2)
{
    int Num = Block1.Num();
    for (int z = 0; z < Num; z++) {
        if (Block1[Num] == 2)
            HISM1->RemoveInstance(z);
        if (Block2[Num] == 2)
            HISM2->RemoveInstance(z);
    }
}

void AChunkManager::AddAChunkOfRegion2Async(int32 lx, int32 ly, FArray_unit8 Block, int ChunkIndex)
{
    UWorld* World = GEngine->GetWorld();

    if (!World)
        return;

    if (!World->IsGameWorld())
        return;

    if (!Block.Block1.IsValidIndex(0) || !Block.Block2.IsValidIndex(0))
        return;

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [=]()
        {
           
            AChunkManager* ChunkManager = Cast<AChunkManager>(UGameplayStatics::GetActorOfClass(World, AChunkManager::StaticClass()));

            if (ChunkManager)
                if (World->IsGameWorld())
                    ChunkManager->AddAChunkOfRegion2(lx, ly, Block, ChunkIndex);
        });
}

bool AChunkManager::CheckToSpawnTriangle(bool Up, FString TriangleName, TArray<FString> ArrayTriangleName, UHexagonHISM* HISM, int& n, FVector& Location)
{
    if (HISM) {
        n = FCString::Atoi(*ArrayTriangleName[3]);
        int X = FCString::Atoi(*ArrayTriangleName[0]);
        int Y = FCString::Atoi(*ArrayTriangleName[1]);
        int Z = FCString::Atoi(*ArrayTriangleName[2]);

        if (Z > 6) return false;



        for (int index = 0; index < RD.ChunkData.Num(); ++index) {
            const TArray<FAsset_Info>& Info1 = RD.ChunkData[index].Info1;

            // Info1Array using a for loop
            for (int InfoIndex = 0; InfoIndex < Info1.Num(); ++InfoIndex) {
                if (Info1[InfoIndex].Name == TriangleName)
                {
                    if (Up) {
                        if (RD.ChunkData[index].Blocks1[InfoIndex] != 1)
                        {
                            //UE_LOG(LogTemp, Warning, TEXT("RD.ChunkData[%d].Blocks1[%d] is not equal to 1"), index, InfoIndex);
                            return false;
                        }
                        else {
                            RD.ChunkData[index].Blocks1[InfoIndex] = 3;
                            //UE_LOG(LogTemp, Log, TEXT("RD.ChunkData[%d].Blocks1[%d] a"), index, InfoIndex);
                            //SaveRegionDataToFile(ChunkName, RD);
                        }
                    }
                    else {
                        if (RD.ChunkData[index].Blocks2[InfoIndex] != 1)
                        {
                           // UE_LOG(LogTemp, Warning, TEXT("RD.ChunkData[%d].Blocks2[%d] is not equal to 1"), index, InfoIndex);
                            return false;
                        }
                        else {
                            RD.ChunkData[index].Blocks2[InfoIndex] = 3;
                            //UE_LOG(LogTemp, Log, TEXT("RD.ChunkData[%d].Blocks2[%d] b"), index, InfoIndex);
                            //SaveRegionDataToFile(ChunkName, RD);
                        }
                    }

               }
            }
        }




        LX = 0.f;
        LY = 0.f;

        // Midle hexagon
        if (X % 2 == 0)
        {
            LX += (sub2);
        }

        // Location X and Y
        LX = LX + (Y * sub);
        LY = X * ro;

        Location = FVector(LX, LY, (Z * Height));
        return true;
    }
    return false;
}

FRandom_Assets AChunkManager::LoadChunkAssetsFromFile(const FString& FilePath)
{
    FString Path = FPaths::ProjectDir() / "RandomAssets" / FilePath;
    FString JsonString;

    if (FFileHelper::LoadFileToString(JsonString, *Path) && !JsonString.IsEmpty())
    {
        FRandom_Assets localL; // Rename the local variable
        FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &localL, 0, 0);
        return localL;
    }

    return FRandom_Assets();
}

bool AChunkManager::CheckJsonFileExists(const FString& FilePath)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + FilePath;
    return FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFilePath);
}

void AChunkManager::SaveChunkRandomAssetsToFile(const FString& FilePath, const FRandom_Assets& FChunkAssets)
{
    FString AbsoluteFilePath = FPaths::ProjectDir() + "/RandomAssets/" + FilePath;

    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(FChunkAssets, JsonString);

    FFileHelper::SaveStringToFile(JsonString, *AbsoluteFilePath);
}


void AChunkManager::Generate_HISM_RandomAsset()
{
    if (!ArrayMesh.IsEmpty())
    {
        for (auto A : ArrayMesh)
        {
            URandomHISM* RHISM = NewObject<URandomHISM>(this, URandomHISM::StaticClass());
            if (RHISM)
            {
                RHISM->RegisterComponent();
                RHISM->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                RHISM->SetStaticMesh(A);
                RHISM->ChunkManager = this;
                //RHISM->SetIsReplicated(true);
                ArrayHISM.Add(RHISM);
            }
        }
    }
}


void AChunkManager::Init_RandomAsset()
{
    Generate_HISM_RandomAsset();

    if (ArrayHISM.IsEmpty()) return;

    // !Exist file
    if (!CheckJsonFileExists("/RandomAssets/" + ChunkName))
    {
        TArray<int> ContainsNumbers;
        for (int Index = 0; Index < ArrayHISM.Num(); ++Index)
        {
            TArray<int> outNumbers;
            TArray<FTransform> t = GenerateRandomArray(ContainsNumbers, 5, outNumbers);
            FRandom_A_Asset RAA;
            RAA.AddNewArray(Index, t, outNumbers);

            RandomAssets.Name = ChunkName;
            int i = RandomAssets.Asset.Add(RAA);
            
        }
        SaveChunkRandomAssetsToFile(ChunkName, RandomAssets);
    }
    else 
    {
        FRandom_Assets FRA = LoadChunkAssetsFromFile(ChunkName);
        RandomAssets = FRA; // replicated
    }

}

void AChunkManager::Check_RandomAsset()
{

    if (!CompletedHex) return;

    if (ArrayHISM.IsEmpty()) Generate_HISM_RandomAsset();

    if (RandomAssets.Asset.IsEmpty()) return;
      
    GenerateRandomAssetsMesh();

    GetWorldTimerManager().ClearTimer(TimerSetRandomAsset);
}

TArray<FTransform> AChunkManager::GenerateRandomArray(TArray<int> ContainsArray, const int& iMax, TArray<int>& outArray)
{
    TArray<int> RandomArray;
    bool bEmpty = ContainsArray.IsEmpty() ? true: false;
    while (RandomArray.Num() < iMax)
    {
        // Generate a random integer
        int RandomNumber = FMath::RandRange(0, 324);

        // Check if RandomNumber is already in the array

        if (bEmpty) 
        {
            if (!RandomArray.Contains(RandomNumber))
                RandomArray.Add(RandomNumber);
        }
        else 
        {
            if (!RandomArray.Contains(RandomNumber) && !ContainsArray.Contains(RandomNumber))
                RandomArray.Add(RandomNumber);
        }

        
    }

    FTransform FT;
    TArray<FTransform> TT;
    for (auto r : RandomArray)
    {
        LX = 0.f;
        LY = 0.f;

        int Z = RD.ChunkData[r].z * Height;

        // Midle hexagon
        if (RD.ChunkData[r].x % 2 == 0)  LX += (sub2);
        

        // Location X and Y
        LX = LX + (RD.ChunkData[r].y * sub);
        LY = RD.ChunkData[r].x * ro;

        float NewScale = FMath::RandRange(1.0f, 1.2f);
        FT = FTransform(FRotator(0, FMath::RandRange(-180.0f, 180.0f), 0),
            FVector((LX) + GetActorLocation().X, (LY) + GetActorLocation().Y, Z),
            FVector(NewScale, NewScale, NewScale));

        TT.Add(FT);
    }
    outArray = RandomArray;
    return TT;
}

FTransform AChunkManager::GenerateRandom_A_Asset(int& OutIndex)
{
    int _Index = -1;

    if (RandomAssets.Asset[0].Indexs.IsEmpty()) // if Empty Array
    {
        // Generate a random integer
        int RandomNumber = FMath::RandRange(0, 341);

        // Check if RandomNumber is already in the array
        if (!RandomAssets.Asset[0].Indexs.Contains(RandomNumber))
            _Index = RandomNumber;
        
    }
    else // if isvalid Array
    {
        while (_Index == -1)
        {
            // Generate a random integer
            int RandomNumber = FMath::RandRange(0, 341);

            // Check if RandomNumber is already in the array
            if (!RandomAssets.Asset[0].Indexs.Contains(RandomNumber))
            {
                _Index = RandomNumber;
                break ;
            }
        }
    }

    if (_Index != -1 && RD.ChunkData.IsValidIndex(_Index)) // Has Random number
    {
        OutIndex = _Index;

       

        LX = 0.f;
        LY = 0.f;

        int Z = RD.ChunkData[_Index].z * Height;

        // Midle hexagon
        if (_X % 2 == 0) LX += (sub2);

        // Location X and Y
        LX = LX + (RD.ChunkData[_Index].y * sub);
        LY = RD.ChunkData[_Index].x * ro;
        float NewScale = FMath::RandRange(1.0f, 1.2f);
        FTransform t = FTransform(FRotator(0, FMath::RandRange(-180.0f, 180.0f), 0),FVector((LX)+GetActorLocation().X, (LY)+GetActorLocation().Y, Z), FVector(NewScale, NewScale, NewScale));
        RandomAssets.Asset[0].AddNew(t, _Index, 30);
        SaveChunkRandomAssetsToFile(ChunkName, RandomAssets);
        Call_Spawn_Tree(OutIndex, t);

        return t;
    }
    OutIndex = -1;
    return FTransform();
}


void AChunkManager::Call_Spawn_Tree(const int HexIndex, const FTransform& t)
{
    MC_Spawn_Tree(HexIndex, t);
}

void AChunkManager::MC_Spawn_Tree_Implementation(const int HexIndex, const FTransform& t)
{
    SpawnARandomAssetsMesh(0, HexIndex, t);
}

void AChunkManager::Check_ReduceDurability_WithHexIndex(URandomHISM* RHISM, const int& Index, int ReduceValue)
{
    
    if (RHISM->Hex_Index.IsValidIndex(Index)) 
    {
        int Hex_Index = RHISM->Hex_Index[Index];

        if (RHISM->ChunkManager->RandomAssets.Asset[0].Durabilitys[Index] <= 1)
        {
            FTransform f = RHISM->ChunkManager->RandomAssets.Asset[0].Transform[Index];
            RHISM->ChunkManager->RandomAssets.Asset[0].RemoveAt(Index);
            RHISM->RemoveInstance(Index);
            SV_Remove_Tree.Broadcast(f, RHISM->ChunkManager, Hex_Index);
        }
        else
        {
            RHISM->ChunkManager->RandomAssets.Asset[0].Durabilitys[Index] -= ReduceValue;
            int n = RHISM->ChunkManager->RandomAssets.Asset[0].Durabilitys[Index];

            MC_Reduce_Durability(RHISM->ChunkManager, n, Index);
        }
        SaveChunkRandomAssetsToFile(ChunkName, RandomAssets);
    }

}

void AChunkManager::Check_ReduceDurability_AllAssets()
{
    for (int32 i = 0; i < RandomAssets.Asset[0].Durabilitys.Num(); ++i)
    {
        int Hex_Index = RandomAssets.Asset[0].Indexs[i];

        if (RandomAssets.Asset[0].Durabilitys[i] <= 1)
        {
            FTransform f = RandomAssets.Asset[0].Transform[i];
            RandomAssets.Asset[0].RemoveAt(i);
            ArrayHISM[0]->RemoveInstance(i);
            SV_Remove_Tree.Broadcast(f, ArrayHISM[0]->ChunkManager, Hex_Index);
        }
        else
        {
            RandomAssets.Asset[0].Durabilitys[i] -= 1;

            MC_Reduce_Durability(ArrayHISM[0]->ChunkManager, i, Hex_Index);
        }
        SaveChunkRandomAssetsToFile(ChunkName, RandomAssets);
    }
}


void AChunkManager::MC_Remove_Tree_Implementation(const FTransform& Transform, AChunkManager* CM, const int& HexIndex, const int& SlotIndex)
{
    if (GIsClient)
    {
        if (CM) {
            int slot = CM->ArrayHISM[0]->Hex_Index.Contains(HexIndex);
            if (slot != -1)
            {
                CM->ArrayHISM[0]->RemoveInstance(slot);
                CM->RandomAssets.Asset[0].RemoveAt(slot);
            }
        }
    }
}

void AChunkManager::MC_Reduce_Durability_Implementation(AChunkManager* CM, const int& Durability, const int& SlotIndex)
{
    if (CM && GIsClient && CM->RandomAssets.Asset[0].Durabilitys.IsValidIndex(SlotIndex))
    {
        CM->RandomAssets.Asset[0].Durabilitys[SlotIndex] = Durability;
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 0 represents the local player index
        if (PC)
        {
            ABeyondOnline2Character* BOC = Cast<ABeyondOnline2Character>(PC->GetPawn());
            if (BOC)
            {
                UAC_Trace* AT = BOC->FindComponentByClass<UAC_Trace>();
                if (AT) 
                {
                    if (AT->Refesh_WidgetInfo.IsBound())
                        AT->Refesh_WidgetInfo.Execute();
                }
            }
        }
    }
}


void AChunkManager::GenerateRandomAssetsMesh()
{
    if (RandomAssets.Asset.IsValidIndex(0))
    {
            for (int i = 0; i < RandomAssets.Asset.Num(); i++)
                        SpawnRandomAssetsMesh(i, RandomAssets.Asset[i].Indexs, RandomAssets.Asset[i].Transform);
                  
    }
}


void AChunkManager::SpawnATriangle(int n, FVector Location, FString TriangleName, UHexagonHISM* HISM)
{
    FTransform t = FTransform(FRotator(0, n * 60, 0), Location, FVector(1, 1, 1));

    // Broadcast the delegate to spawn the triangle
    OnSpawnTriangle.Broadcast(this, t, TriangleName, HISM);
}


void AChunkManager::SpawnARandomAssetsMesh(int32 index, const int HexIndex, const FTransform& Transform)
{
    if (!ArrayHISM.IsEmpty())
        if (ArrayHISM.IsValidIndex(index)) 
        {
            ArrayHISM[index]->AddInstance(Transform, true);
            ArrayHISM[index]->Hex_Index.Add(HexIndex);
        }


}

void AChunkManager::SpawnRandomAssetsMesh(int32 index, const TArray<int> HexIndex, const TArray<FTransform>& ArrayTransform)
{
    if (!ArrayHISM.IsEmpty()) 
    {
        if (ArrayHISM.IsValidIndex(index))
        {
            ArrayHISM[index]->AddInstances(ArrayTransform, true, true);
            ArrayHISM[index]->Hex_Index.Append(HexIndex);

        }
    }
}

void AChunkManager::AddInstanceAsync(UHexagonHISM* HISM, const FTransform& Transform, const FString& TriangleName, const FString& Hexagon_Name, const int& ChunkIndex, const int& Number, const int& Block, const FAsset_Info& Info)
{

    // Create a lambda function to add the instance and store the triangle name
    TFunction<void()> AddInstanceLambda = [=]()
    { 
 
        int32 InstanceIndex = HISM->AddInstance(Transform);
        HISM->Triangle_Names.Add(TriangleName);
        HISM->Hexagon_Name.Add(Hexagon_Name);
        HISM->Number.Add(InstanceIndex);
        HISM->ChunkIndex.Add(ChunkIndex);
        HISM->Info.Add(Info);
        HISM->Blocks.Add(Block);
        HISM->Chunk_Name = RD.RegionName;
    };


    // Execute the lambda function asynchronously
    FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(AddInstanceLambda, TStatId(), nullptr, ENamedThreads::GameThread);
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
}

void AChunkManager::AddInstanceAsync2(UHexagonHISM* HISM, const FChunkInstance CI)
{
    TArray<int> InstanceIndex = HISM->AddInstances(CI.Transform, true, false);

    HISM->Triangle_Names.Append(CI.TriangleName);
    HISM->Hexagon_Name.Append(CI.Hexagon_Name);
    HISM->Number.Append(InstanceIndex);
    HISM->ChunkIndex.Append(CI.ChunkIndex);
    HISM->Info.Append(CI.Info);
    HISM->Blocks.Append(CI.Blocks);
    HISM->Chunk_Name = RD.RegionName;
    
    
}

void AChunkManager::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME conditionally replicates the property if it meets the conditions (e.g., if it's marked for Replication).
    DOREPLIFETIME(AChunkManager, ChunkName);
    DOREPLIFETIME(AChunkManager, OwnerChunk);
    DOREPLIFETIME(AChunkManager, RandomAssets);
    DOREPLIFETIME(AChunkManager, ASpawnRandomManager);
    DOREPLIFETIME(AChunkManager, ArrayMesh);
    
}