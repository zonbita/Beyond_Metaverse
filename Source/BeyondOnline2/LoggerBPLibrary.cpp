// Fill out your copyright notice in the Description page of Project Settings.


#include "LoggerBPLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManagerGeneric.h"

ULoggerBPLibrary::ULoggerBPLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

#pragma region verbosity
void ULoggerBPLibrary::SetLogLevel(UObject* WorldContextObject, ELogLevel eLogLevel)
{
#if !NO_LOGGING
	LogBlueprintUserMessages.SetVerbosity((ELogVerbosity::Type)eLogLevel);
#endif
}

ELogLevel ULoggerBPLibrary::GetLogLevel(UObject* WorldContextObject)
{
#if !NO_LOGGING
	return (ELogLevel)LogBlueprintUserMessages.GetVerbosity();
#else
	return ELogLevel::None;
#endif
}

bool ULoggerBPLibrary::IsLogLevelSuppressed(UObject* WorldContextObject, ELogLevel eLogLevel)
{
#if !NO_LOGGING
	return LogBlueprintUserMessages.IsSuppressed((ELogVerbosity::Type)eLogLevel);
#else
	return true;
#endif
}
#pragma endregion

#pragma region utility functions
void ULoggerBPLibrary::LogInfo(UObject* WorldContextObject, FString InString, bool bPrintToLog, bool bPrintToScreen, float Duration)
{
	ULoggerBPLibrary::Log(WorldContextObject, InString, ELogLevel::Info, bPrintToLog, bPrintToScreen, Duration);
}

void ULoggerBPLibrary::LogWarning(UObject* WorldContextObject, FString InString, bool bPrintToLog, bool bPrintToScreen, float Duration)
{
	ULoggerBPLibrary::Log(WorldContextObject, InString, ELogLevel::Warning, bPrintToLog, bPrintToScreen, Duration);
}

void ULoggerBPLibrary::LogError(UObject* WorldContextObject, FString InString, bool bPrintToLog, bool bPrintToScreen, float Duration)
{
	ULoggerBPLibrary::Log(WorldContextObject, InString, ELogLevel::Error, bPrintToLog, bPrintToScreen, Duration);
}

void ULoggerBPLibrary::LogFatal(UObject* WorldContextObject, FString Message)
{
	ULoggerBPLibrary::Log(WorldContextObject, Message, ELogLevel::Fatal, true, false, 0.0f);
}
#pragma endregion

#pragma region generic function
void ULoggerBPLibrary::Log(UObject* WorldContextObject, FString InString, ELogLevel eLogLevel, bool bPrintToLog, bool bPrintToScreen, float Duration)
{
#if !NO_LOGGING
	FColor logColor = FColor::White;
	switch (eLogLevel)
	{
	case ELogLevel::Warning:
		logColor = FColor::Yellow;
		break;
	case ELogLevel::Error:
	case ELogLevel::Fatal:
		logColor = FColor::Red;
		break;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server: "));
				break;
			case NM_Standalone:
				break;
			}
		}
	}

	const FString FinalDisplayString = Prefix + InString;
	FString FinalLogString = FinalDisplayString;

	static const FBoolConfigValueHelper DisplayPrintStringSource(TEXT("Kismet"), TEXT("bLogPrintStringSource"), GEngineIni);
	if (DisplayPrintStringSource)
	{
		const FString SourceObjectPrefix = FString::Printf(TEXT("[%s] "), *GetNameSafe(WorldContextObject));
		FinalLogString = SourceObjectPrefix + FinalLogString;
	}

	if (bPrintToLog)
	{

		switch (eLogLevel)
		{
		case ELogLevel::Warning:
			UE_LOG(LogBlueprintUserMessages, Warning, TEXT("%s"), *FinalLogString);
			break;
		case ELogLevel::Error:
			UE_LOG(LogBlueprintUserMessages, Error, TEXT("%s"), *FinalLogString);
			break;
		case ELogLevel::Fatal:
			UE_LOG(LogBlueprintUserMessages, Fatal, TEXT("%s"), *FinalLogString);
			break;
		default:
			UE_LOG(LogBlueprintUserMessages, Log, TEXT("%s"), *FinalLogString);
			break;
		}

		APlayerController* PC = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);
		ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : NULL);
		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
		{
			LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalDisplayString);
		}
	}

	// Also output to the screen if possible and hide suppressed log levels
	if (bPrintToScreen && !LogBlueprintUserMessages.IsSuppressed((ELogVerbosity::Type)eLogLevel))
	{
		if (GAreScreenMessagesEnabled)
		{
			if (GConfig && Duration < 0)
			{
				GConfig->GetFloat(TEXT("Kismet"), TEXT("PrintStringDuration"), Duration, GEngineIni);
			}
			GEngine->AddOnScreenDebugMessage((uint64)-1, Duration, logColor, FinalDisplayString);
		}
		else
		{
			UE_LOG(LogBlueprint, Warning, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
		}
	}
#endif
}
#pragma endregion



void ULoggerBPLibrary::WriteLn(FString Text)
{
	static FString w = StartLn(Text);
	if (w.IsEmpty()) {
		StartLn(Text);
	}
	else {

		// Open the file for appending
		FArchive* Ar = IFileManager::Get().CreateFileWriter(*w, FILEWRITE_Append);
		if (Ar)
		{
			// Convert FString to UTF-8 encoded string
			const std::string SaveTextUtf8 = TCHAR_TO_UTF8(*Text);
			// Convert to TArray for writing
			TArray<uint8> SaveTextBytes;
			SaveTextBytes.Append(reinterpret_cast<const uint8*>(SaveTextUtf8.c_str()), SaveTextUtf8.size());
			// Write the text to the file
			Ar->Serialize(const_cast<uint8*>(SaveTextBytes.GetData()), SaveTextBytes.Num());
			// Write a newline character
			Ar->Serialize((uint8*)"\n", 1);
			// Close the file
			delete Ar;

		}
	}
}


FString ULoggerBPLibrary::StartLn(FString SaveTextB)
{
	// Create the full path for the file
	FString Directory = FPaths::ProjectDir() + "/ServerLog/";
	// Get the current date and time
	FDateTime CurrentTime = FDateTime::Now();

	// Extract the real-time components
	int32 Year = CurrentTime.GetYear();
	int32 Month = CurrentTime.GetMonth();
	int32 Day = CurrentTime.GetDay();
	// Print the real-time components
	FString RealTime = FString::Printf(TEXT("%d-%02d-%02d"), Year, Month, Day);

	FString FilePath = Directory + RealTime + ".txt";

	// Check if the file already exists
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);

	// If the file exists, find an available ascending number suffix
	int32 FileSuffix = 0;
	FString NewFileName = RealTime;
	while (bFileExists)
	{
		FileSuffix++;

		NewFileName = FString::Printf(TEXT("%s_%d"), *FPaths::GetBaseFilename(RealTime), FileSuffix);
		FilePath = Directory + NewFileName + ".txt";
		bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	}

	// Create the directories if they don't exist
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		PlatformFile.CreateDirectoryTree(*Directory);
	}


	// Open the file for appending
	FArchive* Ar = IFileManager::Get().CreateFileWriter(*FilePath, FILEWRITE_Append);
	if (Ar)
	{
		// Convert FString to UTF-8 encoded string
		const std::string SaveTextUtf8 = TCHAR_TO_UTF8(*SaveTextB);
		// Convert to TArray for writing
		TArray<uint8> SaveTextBytes;
		SaveTextBytes.Append(reinterpret_cast<const uint8*>(SaveTextUtf8.c_str()), SaveTextUtf8.size());
		// Write the text to the file
		Ar->Serialize(const_cast<uint8*>(SaveTextBytes.GetData()), SaveTextBytes.Num());
		// Write a newline character
		Ar->Serialize((uint8*)"\n", 1);
		// Close the file
		delete Ar;

	}
	return FilePath;
}