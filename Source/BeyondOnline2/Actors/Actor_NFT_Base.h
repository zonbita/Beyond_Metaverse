// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Actor_NFT_Base.generated.h"


UCLASS()
class BEYONDONLINE2_API AActor_NFT_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_NFT_Base(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UAC_NFT* AC_NFT;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UWidgetComponent* WidgetComp;
};
