// Fill out your copyright notice in the Description page of Project Settings.


#include "B_HUD.h"

void AB_HUD::BeginPlay()
{
    Super::BeginPlay();
    //if (wg_GamePlay) {
    //    // Create the user widget
    //    UUserWidget* CreatedWidget = CreateWidget<wg_GamePlay>(GetOwningPlayerController(), AB_HUD::StaticClass());

    //    // Add the user widget to the viewport
    //    CreatedWidget->AddToViewport();

    //    // Assign the created widget to the wg_GamePlay variable
    //    wg_GamePlay = CreatedWidget;
    //}
}

void AB_HUD::ShowAssetInfo_Implementation(const AActor* Actor)
{
}
