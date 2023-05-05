// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
FindSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete)),
JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnJoinSessionComplete)),
DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this,&ThisClass::OnDestroySessionComplete)),
StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem= IOnlineSubsystem::Get();
	if (Subsystem)
	{
		Sessioninterface = Subsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!Sessioninterface.IsValid()) return;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			FColor::Green,
			FString(TEXT("valid!!"))
		);
	}
	FNamedOnlineSession* ExistingSession = Sessioninterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession)
	{
		Sessioninterface->DestroySession(NAME_GameSession);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Green,
				FString(TEXT("destroying!!"))
			);
		}
	}
	//store the delegate in the delegate_handle
	CreateSessionCompleteDelegateHandle = Sessioninterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;
	LastSessionSettings->bUseLobbiesIfAvailable = true;

	ULocalPlayer* LocalPlayer =GetWorld()->GetFirstLocalPlayerFromController();
	if (!Sessioninterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		Sessioninterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Green,
				FString(TEXT("clearing!!"))
			);
		}
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
	
}

void UMultiplayerSessionsSubsystem::FindSession(int32 MaxSearchResult)
{
	if (!Sessioninterface.IsValid()) return;
	FindSessionCompleteDelegateHandle=Sessioninterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResult;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	if (!Sessioninterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		Sessioninterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
		MultiplayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}

}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!Sessioninterface.IsValid())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	JoinSessionCompleteDelegateHandle = Sessioninterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!Sessioninterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		Sessioninterface->ClearOnFindSessionsCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			FColor::Green,
			FString(TEXT("createsessioncomplete!!"))
		);
	}
	if (Sessioninterface.IsValid())
	{
		Sessioninterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	MultiplayerOnCreateSessionComplete.Broadcast(true);
}

void UMultiplayerSessionsSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (Sessioninterface.IsValid())
	{
		Sessioninterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}
	if (LastSessionSearch->SearchResults.Num() == 0)
	{
		MultiplayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	MultiplayerOnFindSessionComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Sessioninterface.IsValid())
	{
		Sessioninterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		
	}
	MultiplayerOnJoinSessionComplete.Broadcast(Result);

}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWassuccsessful)
{
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWassuccsessful)
{
}
