// Fill out your copyright notice in the Description page of Project Settings.


#include "../Components/AC_Inventory.h"
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UAC_Inventory::UAC_Inventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_Inventory::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UAC_Inventory::Init_DataHotBar()
{
	Data_HotBar.Init(-1, 9);
}

void UAC_Inventory::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME conditionally replicates the property if it meets the conditions (e.g., if it's marked for Replication).
    DOREPLIFETIME(UAC_Inventory, ContainerSize);
	DOREPLIFETIME(UAC_Inventory, isShop);

}