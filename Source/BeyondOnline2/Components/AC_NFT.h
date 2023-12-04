// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../BeyondOnline2.h"
#include "AC_NFT.generated.h"


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEYONDONLINE2_API UAC_NFT : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAC_NFT();

protected:
	// Called when the game starts 
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Info")
		FAsset_Info Asset_Info;

	/// Set Outline for static mesh all
	UFUNCTION(BlueprintCallable)
		void SetOutLine(bool Enable);

	bool bShowWidget = false;
};
