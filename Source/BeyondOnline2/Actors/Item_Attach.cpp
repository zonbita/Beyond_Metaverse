// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/Item_Attach.h"
#include <Net/UnrealNetwork.h>

// Sets default values
AItem_Attach::AItem_Attach()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SM->SetEnableGravity(false);
	SM->SetSimulatePhysics(false);
	SetRootComponent(SM);
	DisableComponentsSimulatePhysics();

	bReplicates = true;
}

// Called when the game starts or when spawned
void AItem_Attach::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem_Attach::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME conditionally replicates the property if it meets the conditions (e.g., if it's marked for Replication).
	DOREPLIFETIME(AItem_Attach, Table);
	DOREPLIFETIME(AItem_Attach, SocketName);
	

}