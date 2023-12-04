// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_NFT_Base.h"
#include "../Components/AC_NFT.h"

// Sets default values
AActor_NFT_Base::AActor_NFT_Base(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a new component
	AC_NFT = CreateDefaultSubobject<UAC_NFT>(TEXT("AC_NFT"));

	/// Widget Component
	WidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("WidgetComponent"));
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void AActor_NFT_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_NFT_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
