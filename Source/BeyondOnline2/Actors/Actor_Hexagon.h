// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_Hexagon.generated.h"



UCLASS()
class BEYONDONLINE2_API AActor_Hexagon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_Hexagon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
