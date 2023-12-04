// Fill out your copyright notice in the Description page of Project Settings.


#include "Async_MakeGETAPI.h"

UAsync_MakeGETAPI* UAsync_MakeGETAPI::MakeHttpGetRequest(const FString& Url, const FString& Field)
{
    UAsync_MakeGETAPI* HttpGetRequestNode = NewObject<UAsync_MakeGETAPI>();
    HttpGetRequestNode->Url = Url;
    HttpGetRequestNode->Field = Field;
    return HttpGetRequestNode;
}

UAsync_MakeGETAPI* UAsync_MakeGETAPI::MakeHttpGetRequest2(const FString& Url, const FString& Field)
{
    UAsync_MakeGETAPI* HttpGetRequestNode = UAsync_MakeGETAPI::MakeHttpGetRequest(Url, Field);
    return HttpGetRequestNode;
}

void UAsync_MakeGETAPI::Activate()
{
    HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(Url);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAsync_MakeGETAPI::OnRequestComplete);
    HttpRequest->ProcessRequest();
}

void UAsync_MakeGETAPI::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid())
    {
        FString ResponseStr = Response->GetContentAsString();
        //UE_LOG(LogTemp, Display, TEXT("HTTP GET response: %s"), *ResponseStr);

        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseStr);

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
        {
            // TArray<TSharedPtr<FJsonValue>> JsonArray = JsonObject->GetArrayField(Field);

            FString Description = JsonObject->GetStringField("description");
            Completed.Broadcast(Field, Description);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON data: %s"), *ResponseStr);
        }
    }
    else
    {
        FString ErrorMsg = "HTTP request failed";
        if (Response.IsValid())
        {
            ErrorMsg += ": " + Response->GetContentAsString();
        }

    }
}