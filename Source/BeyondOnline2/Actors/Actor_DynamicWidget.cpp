// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/Actor_DynamicWidget.h"
#include <BeyondOnline2/BeyondOnline2Character.h>

// Sets default values
AActor_DynamicWidget::AActor_DynamicWidget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	SetRootComponent(Widget);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	//Widget->SetHiddenInGame(true);

	// Assuming you have a file path to the widget class
	WidgetClassPath = TEXT("/Game/Blueprints/Widgets/WBP_Key.WBP_Key_C");

	WidgetClass = LoadClass<UUserWidget>(nullptr, *WidgetClassPath);

	// Set the class for the WidgetComponent
	Widget->SetWidgetClass(WidgetClass);

}

// Called when the game starts or when spawned
void AActor_DynamicWidget::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActor_DynamicWidget::ShowWidget(FVector Location, FAsset_Info info)
{
	if (Widget)
	{
		this->SetActorLocation(Location);
	}
}

void AActor_DynamicWidget::HideWidget()
{
	Widget->SetHiddenInGame(true);
}
