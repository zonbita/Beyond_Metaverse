// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Async_MakeGETAPI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHttpRequestCompleted, const FString&, Data, const FString&, Outs);

UCLASS()
class BEYONDONLINE2_API UAsync_MakeGETAPI : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
        static UAsync_MakeGETAPI* MakeHttpGetRequest(const FString& Url, const FString& Field);

    UFUNCTION(BlueprintCallable)
        static UAsync_MakeGETAPI* MakeHttpGetRequest2(const FString& Url, const FString& Field);



    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
        FOnHttpRequestCompleted Completed;

private:
    FString Url, Field;
    TSharedPtr<IHttpRequest> HttpRequest;

    void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
};
