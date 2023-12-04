// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeyondOnline2GameMode.h"
#include "BeyondOnline2Character.h"
#include "UObject/ConstructorHelpers.h"

ABeyondOnline2GameMode::ABeyondOnline2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
