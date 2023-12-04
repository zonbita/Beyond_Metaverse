// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <BeyondOnline2/Components/AC_Inventory.h>
#include "GameFramework/Actor.h"
#include "Item_Base.generated.h"


UCLASS()
class BEYONDONLINE2_API AItem_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* SM;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool Taking;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, meta = (ExposeOnSpawn = "true"))
		bool collision = false;

	UFUNCTION(BlueprintCallable)
	UAC_Inventory* CheckGetItem(APlayerController* PC);

};
