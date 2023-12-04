// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BeyondOnline2.h"
#include "Components/WidgetComponent.h"
#include "Actor_DynamicWidget.generated.h"

UCLASS()
class BEYONDONLINE2_API AActor_DynamicWidget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_DynamicWidget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FString WidgetClassPath;

	TSubclassOf<UUserWidget> WidgetClass;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<UWidgetComponent> Widget;

	UFUNCTION(BlueprintCallable)
	void ShowWidget(FVector Location, FAsset_Info info);

	UFUNCTION(BlueprintCallable)
	void HideWidget();
};
