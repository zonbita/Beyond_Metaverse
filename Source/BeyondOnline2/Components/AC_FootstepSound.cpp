// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_FootstepSound.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values for this component's properties
UAC_FootstepSound::UAC_FootstepSound()
{

}


// Called when the game starts
void UAC_FootstepSound::BeginPlay()
{
	Super::BeginPlay();

	// ...

	C_Audio = NewObject<UAudioComponent>(GetOwner());  // create a new instance of the UAudioComponent
	C_Audio->SetSound(SoundBase);
	C_Audio->SetBoolParameter("step_play", true);
	C_Audio->SetBoolParameter("isMale", true);
	Pawn = Cast<APawn>(GetOwner());
}

void UAC_FootstepSound::TryFootstep(USkeletalMeshComponent* Mesh, bool LeftFoot)
{
	bool HasSurface;
	FVector Location;
	TEnumAsByte<EPhysicalSurface> Surface;

	CheckSurface(Mesh, LeftFoot, HasSurface, Location, Surface);
}

USoundBase* UAC_FootstepSound::GetSplashSound(TEnumAsByte<EPhysicalSurface> FootstepType)
{
	return nullptr;
}

void UAC_FootstepSound::PlayFootstepSound(TEnumAsByte<EPhysicalSurface> Surface)
{
}

void UAC_FootstepSound::SpawnFootstepEffects(USkeletalMeshComponent* MeshComponent, FVector Location, TEnumAsByte<EPhysicalSurface> SurfaceType, bool Left)
{
}

void UAC_FootstepSound::CheckSurface(USkeletalMeshComponent* SkeletalMesh, bool IsLeftFoot, bool& HasSurface, FVector& Location, TEnumAsByte<EPhysicalSurface>& Surface)
{
	FVector newLocation;
	newLocation = SkeletalMesh->GetSocketLocation(IsLeftFoot ? "foot_l" : "foot_r");
	FHitResult Hit;
	bool b = GetWorld()->LineTraceSingleByObjectType(
		Hit,
		newLocation,
		newLocation - (0, 0, 40),
		FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)),
		FCollisionQueryParams("", true, Pawn)

	);

	if (b) {
		
		HasSurface = true;
		Location = Hit.Location;
		UPhysicalMaterial* PhysMat = Hit.PhysMaterial.Get();
		if (PhysMat != nullptr)
		{
			
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMat);
			Surface = SurfaceType;
		}
		return;
	}
	else {
		 HasSurface = false; 
		 Location = FVector(0,0,0);
		 Surface = EPhysicalSurface::SurfaceType1;
		 return;
	}
}

