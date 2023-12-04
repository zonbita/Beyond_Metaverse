// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "AsyncDownloadImage.generated.h"

DECLARE_DELEGATE_OneParam(FOnImageDownloaded, UTexture2D*);
UCLASS()
class BEYONDONLINE2_API AAsyncDownloadImage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsyncDownloadImage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

	TSharedPtr<IHttpRequest> Http;
public:	

	void DownloadAndCheckImageExistence(FString Url, const FOnImageDownloaded& OnImageDownloaded);
	
};
