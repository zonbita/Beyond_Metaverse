// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/Actor_Hexagon.h"


// Sets default values
AActor_Hexagon::AActor_Hexagon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


}

// Called when the game starts or when spawned
void AActor_Hexagon::BeginPlay()
{
	Super::BeginPlay();
	
}
