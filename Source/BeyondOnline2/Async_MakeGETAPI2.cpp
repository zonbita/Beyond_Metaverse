// Fill out your copyright notice in the Description page of Project Settings.


#include "Async_MakeGETAPI2.h"

UAsync_MakeGETAPI2* UAsync_MakeGETAPI2::ReadListFileAsync(const FString& Url)
{
    UAsync_MakeGETAPI2* Action = NewObject<UAsync_MakeGETAPI2>();
    Action->Url = Url;
    return Action;
}

void UAsync_MakeGETAPI2::Activate()
{
    HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(*Url);

    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAsync_MakeGETAPI2::OnHttpRequestComplete);

    HttpRequest->ProcessRequest();
}

void UAsync_MakeGETAPI2::OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        FString FileContent = Response->GetContentAsString();

        TArray<FString> ContentArray;
        FileContent.ParseIntoArray(ContentArray, TEXT("\n"), true);
        // Process the file content as needed
        //UE_LOG(LogTemp, Warning, TEXT("List.txt content:\n%s"), *FileContent);

        // Fire the OnSuccess event
        Completed.Broadcast("", ContentArray);
    }

}