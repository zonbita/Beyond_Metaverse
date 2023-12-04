// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/Actor_DynamicWidget.h"
#include "InputActionValue.h"
#include "Components/AC_Trace.h"
#include "BeyondOnline2Character.generated.h"


UCLASS(config=Game)
class ABeyondOnline2Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** AC_Trace */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UAC_Trace* AC_Trace;



public:
	ABeyondOnline2Character();
	
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	bool bEnablePlaceHexagon = false;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor_DynamicWidget> Actor_DynamicWidgetClass = AActor_DynamicWidget::StaticClass();

	UPROPERTY(BlueprintReadOnly, Category = "Components")
		AActor_DynamicWidget* Actor_DynamicWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UUserWidget* WG;


	UFUNCTION(BlueprintCallable)
		void EnablePlaceHexagon();

		void TracePlaceHexagon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfter_TracePlaceHexagon, FHitResult, Hit);


	/// Call After Trace Hit Complete
	UPROPERTY(BlueprintAssignable)
		FAfter_TracePlaceHexagon After_TracePlaceHexagon;

	FTimerHandle TimerPlaceHexagon;


	UFUNCTION(BlueprintNativeEvent)
		void DisablePlaceHexagon();

	virtual void DisablePlaceHexagon_Implementation();
};

