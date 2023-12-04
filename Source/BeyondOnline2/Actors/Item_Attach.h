// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item_Attach.generated.h"

UCLASS()
class BEYONDONLINE2_API AItem_Attach : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Attach();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UStaticMeshComponent* SM;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		FDataTableRowHandle Table;
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, meta = (ExposeOnSpawn = "true"))
		FName SocketName;
};
