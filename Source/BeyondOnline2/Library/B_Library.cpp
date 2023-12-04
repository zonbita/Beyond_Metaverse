// Fill out your copyright notice in the Description page of Project Settings.


#include "../Library/B_Library.h"
#include "SocketSubsystem.h"
#include "Misc/Guid.h"

FString UB_Library::GetIP()
{
    bool bCanBindAll;
    TSharedPtr<class FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
    FString IP = Addr->ToString(false);
    return IP;
}

void UB_Library::ConvertMapToStringArray(const FCompountTags& TagValueArray, TArray<FString>& Tags, TArray<FString>& Value)
{
    // Access the wrapped map and iterate through its values
    for (const TPair<FString, FString>& KeyValue : TagValueArray.TagValue)
    {
        Tags.Add(KeyValue.Key);
        Value.Add(KeyValue.Value);
    }
}

void UB_Library::ConvertCompoundTagsToData(const TArray<FCompountTags>& CompountTags, TArray<FCompountTags2>& Data)
{
    for (const FCompountTags& c : CompountTags)
    {
        TArray<FString> Tags;
        FCompountTags2 CT;
        ConvertMapToStringArray(c, Tags, CT.TagValue);
        CT.Name = c.Name;

        Data.Add(CT);
    }
}

TArray<FString> UB_Library::GetAllValueOfTriangleName(FString Name)
{
    // Split the input string using underscores as separators
    TArray<FString> StringParts;
    Name.ParseIntoArray(StringParts, TEXT("_"), true);

    return StringParts;
}

FString UB_Library::ConvertArrayToTriangleName(TArray<FString> ArrayName)
{
    FString ResultString = FString::Join(ArrayName, TEXT("_"));
    return ResultString;
}

void GenerateRandomUUID()
{
    // Generate a random UUID
    FGuid UUID = FGuid::NewGuid();

    // Convert the UUID to a string
    FString UUIDString = UUID.ToString();

    // Print the generated UUID
    //UE_LOG(LogTemp, Warning, TEXT("Random UUID: %s"), *UUIDString);
}