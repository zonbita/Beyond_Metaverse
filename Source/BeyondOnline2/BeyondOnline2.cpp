// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeyondOnline2.h"
#include "SocketSubsystem.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, BeyondOnline2, "BeyondOnline2");

const FString GetIP()
{
	bool bCanBindAll;
	TSharedPtr<class FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
	FString IP = Addr->ToString(false);
	return IP;
}
