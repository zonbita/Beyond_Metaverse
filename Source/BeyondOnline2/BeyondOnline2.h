// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Modules/ModuleInterface.h"
#include "BeyondOnline2.generated.h"



USTRUCT(BlueprintType)
struct FEntityData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Entity")
		FString EntityType;

	UPROPERTY(BlueprintReadWrite, Category = "Entity")
		FVector Position = FVector(0,0,0);

	UPROPERTY(BlueprintReadWrite, Category = "Entity")
		FRotator Rotation = FRotator(0, 0, 0);

	UPROPERTY(BlueprintReadWrite, Category = "Entity")
		float Scale = 1;
};

USTRUCT(Blueprintable)
struct FAsset_Info {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		float Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		int Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		int NumTris;



	FAsset_Info() {
		Name = "";
		Owner = "";
		Durability = 100;
		Price = 0;
		NumTris = 0;
	}
};

USTRUCT(Blueprintable)
struct FRandom_A_Asset {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random A Asset")
		int32 ID_Mesh = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random A Asset")
		TArray<FTransform> Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random A Asset")
		TArray<int> Indexs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random A Asset")
		TArray<int> Durabilitys;


	void AddNewArray(const int& _ID_Mesh, const TArray<FTransform>& _Transform, const TArray<int>& _Indexs)
	{
		ID_Mesh = _ID_Mesh;
		Transform.Append(_Transform);
		Indexs.Append(_Indexs);
		Durabilitys.Init(30,Indexs.Num());
	}
	void AddNew(const FTransform& _Transform, const int& _Indexs, const int& _Durabilitys)
	{
		Transform.Add(_Transform);
		Indexs.Add(_Indexs);
		Durabilitys.Add(30);
	}
	void RemoveAt(const int& slot) 
	{
		Transform.RemoveAt(slot);
		Indexs.RemoveAt(slot);
		Durabilitys.RemoveAt(slot);
	}
};

USTRUCT(Blueprintable)
struct FRandom_Assets {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random_Assets")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random_Assets")
		TArray<FRandom_A_Asset> Asset;

	FRandom_Assets() {
		Name = "";
		Asset.Empty();
	}

	void AddNew(const FString&  Chunkname,const int& _ID_Mesh, const TArray<FTransform>& _Transform, const TArray<int>& _Indexs)
	{
		Name = Chunkname;
		FRandom_A_Asset A;
		A.AddNewArray(_ID_Mesh, _Transform, _Indexs);
		Asset.Add(A);
	}
};

USTRUCT(BlueprintType)
struct FTagValueMap
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TMap<FString, FString> TagValueMap;
};

USTRUCT(Blueprintable)
struct FCompountNameChild3 {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Value;
};

USTRUCT(Blueprintable)
struct FCompountNameChild2 {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FCompountNameChild3 TagValue;
};

USTRUCT(Blueprintable)
struct FCompountNameChild {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TArray<FCompountNameChild2> N;
};

USTRUCT(Blueprintable)
struct FCompountName {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TMap<FString, FString> TagValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TArray<FCompountNameChild> C2;
};

USTRUCT(Blueprintable)
struct FCompountTags {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TMap<FString, FString> TagValue;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
	//	TArray<FCompountName> C1;
};

USTRUCT(Blueprintable)
struct FCompountTags2 {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TArray<FString> TagValue;
};

USTRUCT(Blueprintable)
struct FCompountContainer {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TArray<FCompountTags> Data;
};

USTRUCT(Blueprintable)
struct FCompountContainer2 {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TArray<FCompountTags2> Data;
};

UENUM(BlueprintType)
enum class EBlock : uint8
{
	Null UMETA(DisplayName = "Null"),
	Air UMETA(DisplayName = "Air"),
	Stone UMETA(DisplayName = "Stone"),
	Dirt UMETA(DisplayName = "Dirt"),
	Grass UMETA(DisplayName = "Grass")
};

USTRUCT(Blueprintable)
struct FArray_unit8 {
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TArray<uint8> Block1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Infomation")
		TArray<uint8> Block2;
};


USTRUCT(Blueprintable)
struct FHexagonData
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FAsset_Info> HexagonInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<EBlock> Blocks;
	void New(FString name);
};
inline void FHexagonData::New(FString name)
{
	FAsset_Info f;
	f.Name = name;
	f.Durability = 100;
	f.NumTris = 200;
	f.Owner = "None";
	f.Price = 200;
	HexagonInfo.Add(f);
	Blocks.Add(EBlock::Dirt);
}

USTRUCT(Blueprintable)
struct FChunkInstance
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FTransform> Transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FString> TriangleName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FString> Hexagon_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<int> ChunkIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FAsset_Info> Info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		mutable TArray<uint8> Blocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FString> ChunkName;
};


USTRUCT(Blueprintable)
struct FChunkData
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Data")
		int x = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Data")
		int y = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Data")
		int z = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FAsset_Info> Info1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		TArray<FAsset_Info> Info2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		mutable TArray<uint8> Blocks1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon Data")
		mutable TArray<uint8> Blocks2;
	void Clear();
};

inline void FChunkData::Clear()
{
	Info1.Empty();
	Info2.Empty();
	Blocks1.Empty();
	Blocks2.Empty();
}


USTRUCT(Blueprintable)
struct FRegionData
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Region Data")
		FString RegionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Region Data")
		TArray<FChunkData> ChunkData;
	void Clear();
};


inline void FRegionData::Clear()
{
	ChunkData.Empty();
}


USTRUCT(Blueprintable)
struct FChunkAssetsChild
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Assets")
		TArray<FTransform> Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Assets")
		TArray<FAsset_Info> Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Assets")
		TArray<FCompountContainer> Tags;
	void Clear();
};

inline void FChunkAssetsChild::Clear()
{
	Transform.Empty();
	Info.Empty();
	Tags.Empty();
}


USTRUCT(Blueprintable)
struct FChunkAssets
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Assets")
		TArray<FString> Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Assets")
		TArray<FChunkAssetsChild> Assets;
};


USTRUCT(Blueprintable)
struct FGLTF_Transform
{
	GENERATED_BODY();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Assets")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chunk Assets")
		TArray<FTransform> GLTF_Transform;
};


USTRUCT(BlueprintType)
struct FOwnerRegion
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Owner Region")
		FString RegionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Owner Region")
		FString PlayerName;
};


USTRUCT(BlueprintType)
struct FPlayerDataInfo
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		int IDCharacter = 0;
};


UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	None,
	Opaque,
	ClearCoat,
	TwoSidedClearCoat,
	Translucent,
	TwoSided,
	TwoSidedTranslucent,
	Masked,
	TwoSidedMasked
};



USTRUCT(BlueprintType)
struct FColorMaterials
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		FString ComponnentName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		int TableIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		FName RowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
		FLinearColor Color = FLinearColor(0.6f, 0.6f, 0.6f);

};