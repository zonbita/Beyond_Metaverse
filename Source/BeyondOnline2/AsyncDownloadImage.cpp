// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncDownloadImage.h"


// Sets default values
AAsyncDownloadImage::AAsyncDownloadImage()
{
	Http = FHttpModule::Get().CreateRequest();

}

// Called when the game starts or when spawned
void AAsyncDownloadImage::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAsyncDownloadImage::DownloadAndCheckImageExistence(FString Url, const FOnImageDownloaded& OnImageDownloaded)
{


    //TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http.ToSharedRef();
    //Request->OnProcessRequestComplete().BindLambda([this, Url, OnImageDownloaded](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bWasSuccessful)
    //    {
    //        if (bWasSuccessful && HttpResponse.IsValid())
    //        {
    //            int32 ResponseCode = HttpResponse->GetResponseCode();

    //            if (ResponseCode == 200)
    //            {
    //                TArray<uint8> ImageData = HttpResponse->GetContent();
    //                // Initialize the ImageWrapperModule
    //                IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
    //                // Load the image using UE4's ImageWrapper module
    //                TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG); // Adjust format as needed
    //                if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
    //                {
    //                    TArray<uint8> UncompressedData;
    //                    if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedData))
    //                    {
    //                        int32 Width = ImageWrapper->GetWidth();
    //                        int32 Height = ImageWrapper->GetHeight();

    //                        // Create a UE4 Texture2D
    //                        UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    //                        if (Texture)
    //                        {
    //                            // Lock the texture for writing
    //                            uint8* TextureData = static_cast<uint8*>(Texture->PlatformData->Mips[0].BulkData.LockReadOnly());
    //                            FMemory::Memcpy(TextureData, UncompressedData.GetData(), UncompressedData.Num());

    //                            // Update mipmaps
    //                            Texture->UpdateResource();

    //                            // Notify the callback that the image is ready
    //                            OnImageDownloaded.ExecuteIfBound(Texture);
    //                        }
    //                    }
    //                }
    //            }
    //            else if (ResponseCode == 404)
    //            {
    //                // The file does not exist
    //                OnImageDownloaded.ExecuteIfBound(nullptr); // Pass nullptr to indicate that the image doesn't exist
    //            }
    //            else
    //            {
    //                // Handle other response codes as needed
    //                OnImageDownloaded.ExecuteIfBound(nullptr); // Pass nullptr for other response codes
    //            }
    //        }
    //        else
    //        {
    //            // Handle the case where the request was not successful
    //            OnImageDownloaded.ExecuteIfBound(nullptr); // Pass nullptr for errors
    //        }
    //    });

    //Request->SetURL(Url);
    //Request->SetVerb(TEXT("GET"));
    //Request->ProcessRequest();

}


