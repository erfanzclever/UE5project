// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GameState)
	{
		int32 NumberOfPlayers=GameState.Get()->PlayerArray.Num();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 15.f,
				FColor::Yellow,
				FString::Printf(TEXT("playersInGame: %d"), NumberOfPlayers)
			);
		}
		APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
		if (PlayerState)
		{
			FString PlayerName=PlayerState->GetPlayerName();
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 15.f,
					FColor::Yellow,
					FString::Printf(TEXT("playerjoined the game %s"), *PlayerName)
				);
			}
		}
		

	}
}
void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f,
			FColor::Yellow,
			FString::Printf(TEXT("playersInGame: %d"), NumberOfPlayers-1)
		);
	}

	APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		FString PlayerName = PlayerState->GetPlayerName();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 15.f,
				FColor::Yellow,
				FString::Printf(TEXT("player left the game %s"), *PlayerName)
			);
		}
	}

}


