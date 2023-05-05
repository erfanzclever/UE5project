// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h" 
#include "MultiplayerSessionsSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionComplete,const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public: 
	UMultiplayerSessionsSubsystem();

	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSession(int32 MaxSearchResult);
	void JoinSession (const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();


	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionComplete MultiplayerOnFindSessionComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWassuccsessful);
	void OnStartSessionComplete(FName SessionName, bool bWassuccsessful);

private:
		IOnlineSessionPtr Sessioninterface;
		TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
		TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

		//
		// to add to the online session interface delegate list;
		//  
		//
		FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
		FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
		FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
		FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
		FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

		FDelegateHandle CreateSessionCompleteDelegateHandle;
		FDelegateHandle FindSessionCompleteDelegateHandle;
		FDelegateHandle JoinSessionCompleteDelegateHandle;
		FDelegateHandle DestroySessionCompleteDelegateHandle;
		FDelegateHandle StartSessionCompleteDelegateHandle;


		int debug2=0;
};
