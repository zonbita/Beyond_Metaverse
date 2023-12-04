// Fill out your copyright notice in the Description page of Project Settings.


#include "../GameMode/SocketServer.h"

// Sets default values
ASocketServer::ASocketServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASocketServer::BeginPlay()
{
	Super::BeginPlay();
    StartServer();
}

void ASocketServer::StartServer()
{
    SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (SocketSubsystem)
    {
        ListenerSocket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("default"), false);
        ListenEndpoint = FIPv4Endpoint(FIPv4Address(127, 0, 0, 1), 5000);

        bool bBound = ListenerSocket->Bind(*ListenEndpoint.ToInternetAddr());
        if (bBound)
        {
            ListenerSocket->Listen(8);
            ListenerSocket->SetRecvErr(true);
            ListenerSocket->SetNonBlocking(true);
            UE_LOG(LogTemp, Warning, TEXT("Server started on port 3000."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to bind server socket."));
        }
    }
}

void ASocketServer::OnConnectionAccepted(FSocket* NewSocket, const FIPv4Endpoint& InClientEndpoint)
{
    FString Message = FString::Printf(TEXT("Level: YourLevel, Position: 1.0 2.0 3.0")); // Replace with actual data
    int32 SentBytes = 0;
    bool bSuccess = NewSocket->Send((uint8*)TCHAR_TO_UTF8(*Message), Message.Len(), SentBytes);
    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sent data to client."));
    }

    // Close the socket after sending data
    NewSocket->Close();
    ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(NewSocket);
}