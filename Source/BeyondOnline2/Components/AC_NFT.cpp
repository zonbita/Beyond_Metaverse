// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_NFT.h"


// Sets default values for this component's properties
UAC_NFT::UAC_NFT()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAC_NFT::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UAC_NFT::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UAC_NFT::SetOutLine(bool Enable)
{
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetOwner()->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	for (UStaticMeshComponent* s : StaticMeshComponents)
	{
		// Do something with the StaticMeshComp, such as setting its outline material
		s->SetRenderCustomDepth(Enable);
		Enable ? s->CustomDepthStencilValue = 1 : s->CustomDepthStencilValue = 0;
	}
}

