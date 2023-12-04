// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Interface/I_Hud.h"
#include "B_HUD.generated.h"

/**
 * 
 */
UCLASS()
class BEYONDONLINE2_API AB_HUD : public AHUD, public II_Hud
{
	GENERATED_BODY()

protected:
	// Called when the game starts 
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	virtual	void ShowAssetInfo_Implementation(const AActor* Actor);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	class UUserWidget* wg_GamePlay;
};
