// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Pawn.h"
#include "AC_FootstepSound.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEYONDONLINE2_API UAC_FootstepSound : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UAC_FootstepSound();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	

	/** Try to do footstep actions for target foot. */
	UFUNCTION(BlueprintCallable, Category = "Default")
		void TryFootstep(USkeletalMeshComponent* Mesh, bool LeftFoot);

	/** Returns splash sound for target footstep type. */
	UFUNCTION(BlueprintPure, Category = "Default")
		USoundBase* GetSplashSound(TEnumAsByte<EPhysicalSurface> FootstepType);

	/** Play footstep sound in the target location. */
	UFUNCTION(BlueprintCallable, Category = "Default")
		void PlayFootstepSound(TEnumAsByte<EPhysicalSurface> Surface);

	/** Spawn footstep decal and particle in the target location. */
	UFUNCTION(BlueprintCallable, Category = "Default")
		void SpawnFootstepEffects(USkeletalMeshComponent* MeshComponent, FVector Location, TEnumAsByte<EPhysicalSurface> SurfaceType, bool Left);

	/** Returns true if there is a surface under the feet. Also returns information about the surface. */
	UFUNCTION(BlueprintCallable, Category = "Default")
		void CheckSurface(USkeletalMeshComponent* SkeletalMesh, bool IsLeftFoot, bool& HasSurface, FVector& Location, TEnumAsByte<EPhysicalSurface>& Surface);
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
		TObjectPtr<UAudioComponent> C_Audio;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
		bool IsMale;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
		TObjectPtr<APawn> Pawn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (MultiLine = "true", ExposeOnSpawn = "true"))
		bool Enable_Effect;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
		USoundBase* SoundBase;
};
