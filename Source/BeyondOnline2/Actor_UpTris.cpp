#include "Actor_UpTris.h"



// Sets default values
AActor_UpTris::AActor_UpTris(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	/// Widget Component
	UScene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
	RootComponent = UScene;
	// Create a new component
	AC_NFT = CreateDefaultSubobject<UAC_NFT>(TEXT("AC_NFT"));
	AC_NFT->bShowWidget = true;


}

void AActor_UpTris::BeginPlay()
{
	Super::BeginPlay();
}
