// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../BeyondOnline2.h"
#include "B_Library.generated.h"



/**
 * 
 */
UCLASS()
class BEYONDONLINE2_API UB_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Network")
		static FString GetIP();

	UFUNCTION(BlueprintCallable, Category = "TArray")
		static void ConvertMapToStringArray(const FCompountTags& TagValueArray, TArray<FString>& Tags, TArray<FString>& Value);

	UFUNCTION(BlueprintCallable, Category = "TArray")
		static void ConvertCompoundTagsToData(const TArray<FCompountTags>& CompountTags, TArray<FCompountTags2>& Data);

	UFUNCTION(BlueprintCallable, Category = "Hexagon")
		static TArray<FString> GetAllValueOfTriangleName(FString Name);

	UFUNCTION(BlueprintCallable, Category = "Hexagon")
		static FString ConvertArrayToTriangleName(TArray<FString> ArrayName);
};
