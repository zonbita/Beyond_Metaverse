// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Async_MakeGETAPI2.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComplete, const FString&, Data, const TArray<FString>&, Outs);

UCLASS()
class BEYONDONLINE2_API UAsync_MakeGETAPI2 : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:
    UFUNCTION(BlueprintCallable, Category = "HTTP")
        static UAsync_MakeGETAPI2* ReadListFileAsync(const FString& Url);

    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
        FOnComplete Completed;

private:
    void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

    TSharedPtr<IHttpRequest> HttpRequest;

    FString Url;

};
