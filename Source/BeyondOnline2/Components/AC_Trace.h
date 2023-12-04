// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Camera/CameraComponent.h>
#include "../BeyondOnline2.h"
#include "../Actors/Actor_DynamicWidget.h"
#include "AC_Trace.generated.h"

// Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShow_TriangleInfo, FVector, SetLocation, FAsset_Info, Info);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShow_RandomAssetInfo, FVector, SetLocation, int, Durability);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHide_WidgetInfo);

DECLARE_DELEGATE(FRefesh_WidgetInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfter_TraceSignature, FHitResult, Hit);


class UAC_NFT;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEYONDONLINE2_API UAC_Trace : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts 
	virtual void BeginPlay() override;



private:
	AActor* Hit_Actor;

	/// Set view NFT Mode
	void SetViewMode();

	// Hide NFT Outline
	void Check_Info(FHitResult Hit);

	void Check_Info_NFT(UAC_NFT* NFT);

public:
	// Sets default values for this component's properties
	UAC_Trace();

	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Range = 700;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShowTrace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int HitItem = -1;

	/// Trace NFT Actor to show widget key
	UFUNCTION(BlueprintCallable)
		void Trace_H();

	// Event Trace
	UFUNCTION(BlueprintCallable)
		void Trace(float newRange, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bShowTrace, bool& True, FHitResult& Hit);



	FRefesh_WidgetInfo Refesh_WidgetInfo;


	/// Call After Trace Hit Complete
	UPROPERTY(BlueprintAssignable)
		FAfter_TraceSignature After_Trace;

	// Function signature Hide Widget Info
	UPROPERTY(BlueprintAssignable)
		FShow_TriangleInfo Show_TriangleInfo;

	UPROPERTY(BlueprintAssignable)
		FShow_RandomAssetInfo Show_RandomAssetInfo;

	// Function signature Hide Widget Info
	UPROPERTY(BlueprintAssignable)
		FHide_WidgetInfo Hide_WidgetInfo;

	// Mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bViewNft_Mode = false;

	/// Call Set view NFT Mode
	UFUNCTION(BlueprintCallable)
		void CallSetViewMode();

	void Refesh_Trace();
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor_DynamicWidget* DynamicWidget;

	FTimerHandle TimerTrace;
};
