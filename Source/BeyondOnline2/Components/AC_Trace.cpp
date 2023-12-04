// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Trace.h"
#include "AC_NFT.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <BeyondOnline2/HexGridManager.h>
#include "../Actors/ChunkManager.h"

UAC_Trace::UAC_Trace()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UAC_Trace::BeginPlay()
{
	Super::BeginPlay();

	// Get component camera
	if (!Camera)
		Camera = GetOwner()->FindComponentByClass<UCameraComponent>();


	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the actor
		DynamicWidget = World->SpawnActor<AActor_DynamicWidget>(AActor_DynamicWidget::StaticClass(), SpawnParams);
	}
	Refesh_WidgetInfo.BindUObject(this, &UAC_Trace::Refesh_Trace);

	GetWorld()->GetTimerManager().SetTimer(TimerTrace, this, &UAC_Trace::Trace_H, .05, true);

}

void UAC_Trace::CallSetViewMode()
{
	bViewNft_Mode = !bViewNft_Mode;
	SetViewMode();
}

void UAC_Trace::Refesh_Trace()
{
	Hit_Actor = NULL;
}

void UAC_Trace::Trace_H()
{
	if (!Camera) return;

	// Make Ignore Actors
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(GetOwner(), 1);

	// Get camera location
	FVector Location = Camera->K2_GetComponentLocation();

	// Hit
	FHitResult Hit;

	UAC_NFT* NFT;

	// Check Server Trace
	bool c = UKismetSystemLibrary::SphereTraceSingle(this,
		Location,
		Location + (Camera->GetForwardVector() * Range),
		2.0f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignoreActors,
		ShowTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		Hit,
		true);

	if (c)
	{
		NFT = Hit.GetActor()->FindComponentByClass<UAC_NFT>();

		if(NFT) // Has NFT Component
		{
			if (Hit.GetActor() != Hit_Actor)  // difference 
			{
				if (Hit_Actor) // Old Actor will disable Outline
				{
					UAC_NFT* N = Hit_Actor->FindComponentByClass<UAC_NFT>();
					if (Hit_Actor->FindComponentByClass<UAC_NFT>()) N->SetOutLine(false);
				}
				HitItem = -1;
				Hit_Actor = Hit.GetActor();
				Check_Info_NFT(NFT); // Replace with new actor and enable outline
			}
		}
		else // No has NFT Component
		{
			if (Hit_Actor)  // Old Actor will disable Outline
			{
				NFT = Hit_Actor->FindComponentByClass<UAC_NFT>();
				if (NFT)
				{
					NFT->SetOutLine(false);
				}
			}

			if (Hit_Actor == Hit.GetActor())
			{
				if (HitItem != Hit.Item)
				{
					HitItem = Hit.Item;
					Check_Info(Hit);
				}
			}
			else {
				if (Hit.GetActor()) // Replace with a new actor
				{
					HitItem = -1;
					Hit_Actor = Hit.GetActor();

					if (HitItem != Hit.Item)
					{
						HitItem = Hit.Item;
						Check_Info(Hit);
					}
				}
				else Hit_Actor = NULL;
			}
		}


	}
	else 
	{
		if (Hit_Actor) 
		{
			NFT = Hit_Actor->FindComponentByClass<UAC_NFT>();
			if (NFT) NFT->SetOutLine(false);
		}

		Hit_Actor = NULL;
		HitItem = -1;
		//Hide_WidgetInfo.Broadcast();
	}
}

void UAC_Trace::SetViewMode()
{
	if (bViewNft_Mode)
	{
		Hit_Actor = NULL;
		HitItem = -1;
		Trace_H();
		DynamicWidget->HideWidget();
	}
	else 
	{
		Hide_WidgetInfo.Broadcast();

		if (Hit_Actor) 
		{
			UAC_NFT* NFT = Hit_Actor->FindComponentByClass<UAC_NFT>();
			if (NFT) NFT->SetOutLine(false);
		}

		DynamicWidget->Widget->SetHiddenInGame(false);
	}
}

void UAC_Trace::Check_Info(FHitResult Hit)
{
	if (Hit_Actor) 
	{

		// Attempt to cast Hit_Actor to HexGridManager
		AChunkManager* ChunkManager = Cast<AChunkManager>(Hit_Actor);
		URandomHISM* RISM = Cast<URandomHISM>(Hit.Component);

		if (RISM)
		{
			if (RISM->ChunkManager)
			{
				if (RISM->ChunkManager->RandomAssets.Asset[0].Durabilitys.IsValidIndex(Hit.Item))
					Show_RandomAssetInfo.Broadcast(Hit.Location, RISM->ChunkManager->RandomAssets.Asset[0].Durabilitys[Hit.Item]);
			}
		}
		else if (ChunkManager)
		{
			UHexagonHISM* HISM = Cast<UHexagonHISM>(Hit.Component);

			if (HISM) 
			{
				int ArraySize = HISM->Info.Num();

				if (Hit.Item >= 0 && Hit.Item < ArraySize)
				{
					FTransform t; 
					HISM->GetInstanceTransform(Hit.Item, t, true);
					Show_TriangleInfo.Broadcast(t.GetLocation(), HISM->Info[Hit.Item]);
				}
			}

		}

	}
}

void UAC_Trace::Check_Info_NFT(UAC_NFT* NFT)
{
	if (Hit_Actor)
	{
		if (bViewNft_Mode && NFT)
			NFT->SetOutLine(true);
		else 
			NFT->SetOutLine(false);

		Show_TriangleInfo.Broadcast(Hit_Actor->GetActorLocation(), NFT->Asset_Info);
	}
}

void UAC_Trace::Trace(float newRange, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bShowTrace, bool& True, FHitResult& Hit)
{
	FHitResult Hits;
	FVector Location = Camera->K2_GetComponentLocation();

	True = UKismetSystemLibrary::LineTraceSingle(this, Location, Location + Camera->GetForwardVector() * Range, TraceChannel, bTraceComplex, ActorsToIgnore, ShowTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, Hits, true);
	if (True)
	{
		Hit = Hits;
	}
	else {
		True = false;
	}
}
