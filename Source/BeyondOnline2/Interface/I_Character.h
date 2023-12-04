// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <BeyondOnline2/Actors/Item_Attach.h>
#include "I_Character.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Character : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BEYONDONLINE2_API II_Character
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Interface")
		void I_Get_ItemUse(TArray<AItem_Attach*>& Actors);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Interface")
		void I_Set_ItemUse(AItem_Attach* Actor, int Slot);
};
