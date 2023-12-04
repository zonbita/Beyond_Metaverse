// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

#include "SocketServer.generated.h"

UCLASS()
class BEYONDONLINE2_API ASocketServer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASocketServer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	FSocket* ListenerSocket;
	ISocketSubsystem* SocketSubsystem;
	FIPv4Endpoint ListenEndpoint;

	UFUNCTION(BlueprintCallable)
		void StartServer();

	void OnConnectionAccepted(FSocket* NewSocket, const FIPv4Endpoint& InClientEndpoint);


    //UFUNCTION(BlueprintCallable, Category = "Socket")
    //    void GetPlayerStatistics() {
    //    // Assume you have a way to get player name and position
    //    FString PlayerName = "John Doe";
    //    FVector PlayerPosition = FVector(100, 200, 300); // Example position

    //    // Create a JSON object to hold player data
    //    TSharedPtr<FJsonObject> PlayerData = MakeShareable(new FJsonObject);
    //    PlayerData->SetStringField("name", PlayerName);

    //    // Convert FVector to a JSON array
    //    TArray<TSharedPtr<FJsonValue>> PositionArray;
    //    PositionArray.Add(MakeShareable(new FJsonValueNumber(PlayerPosition.X)));
    //    PositionArray.Add(MakeShareable(new FJsonValueNumber(PlayerPosition.Y)));
    //    PositionArray.Add(MakeShareable(new FJsonValueNumber(PlayerPosition.Z)));
    //    PlayerData->SetArrayField("position", PositionArray);

    //    // Serialize PlayerData to JSON string
    //    FString JsonString;
    //    TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&JsonString);
    //    FJsonSerializer::Serialize(PlayerData.ToSharedRef(), Writer);

    //    // Assuming you've set up socket communication
    //    if (SocketIO.IsValid() && SocketIO->IsConnected()) {
    //        SocketIO->Emit(FString("player_statistics"), JsonString);
    //    }
    //}
};
