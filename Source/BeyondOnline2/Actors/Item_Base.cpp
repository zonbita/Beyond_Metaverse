// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actors/Item_Base.h"
#include <Net/UnrealNetwork.h>

// Sets default values
AItem_Base::AItem_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SM->SetEnableGravity(false);
	SetRootComponent(SM);
}

// Called when the game starts or when spawned
void AItem_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

UAC_Inventory* AItem_Base::CheckGetItem(APlayerController* PC)
{

	if (PC) {
		UAC_Inventory* ACI = PC->FindComponentByClass<UAC_Inventory>();
		return ACI;
	}


	return NULL;
}

void AItem_Base::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME conditionally replicates the property if it meets the conditions (e.g., if it's marked for Replication).
	DOREPLIFETIME(AItem_Base, collision);

}