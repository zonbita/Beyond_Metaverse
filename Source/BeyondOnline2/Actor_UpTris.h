#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include "./Components/AC_NFT.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Actor_UpTris.generated.h"


UCLASS(BlueprintType)
class BEYONDONLINE2_API AActor_UpTris : public AActor
{
	GENERATED_BODY()

	
public:
	// Sets default values for this actor's properties
	AActor_UpTris(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AC_NFT")
		class UAC_NFT* AC_NFT;

	UPROPERTY(EditAnywhere, Category = "Components")
		class USceneComponent* UScene;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
