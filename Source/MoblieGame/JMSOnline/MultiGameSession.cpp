// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiGameSession.h"


#include "MultiPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineStatsInterface.h"


AMultiGameSession::AMultiGameSession()
{


}


void AMultiGameSession::CreateSession(FName KeyName, FString KeyValue) // Dedicated Server Only
{
    // Tutorial 3: This function will create an Multi Session.
 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface(); // 세션 인터페이스



    // Bind delegate to callback function
    CreateSessionDelegateHandle =
        Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleCreateSessionCompleted));

    // Set session settings 
    TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
    SessionSettings->NumPublicConnections = MaxNumberOfPlayersInSession; //We will test our sessions with 2 players to keep things simple
    SessionSettings->bShouldAdvertise = true; //This creates a public match and will be searchable. This will set the session as joinable via presence. 
    SessionSettings->bUsesPresence = true;   //No presence on dedicated server. This requires a local user.
    SessionSettings->bAllowJoinViaPresence = false; // superset by bShouldAdvertise and will be true on the backend
    SessionSettings->bAllowJoinViaPresenceFriendsOnly = false; // superset by bShouldAdvertise and will be true on the backend
    SessionSettings->bAllowInvites = false;    //Allow inviting players into session. This requires presence and a local user. 
    SessionSettings->bAllowJoinInProgress = false; //Once the session is started, no one can join.
    SessionSettings->bIsDedicated = true; //Session created on dedicated server.
    SessionSettings->bUseLobbiesIfAvailable = false; //This is an Multi Session not an Multi Lobby as they aren't supported on Dedicated Servers.
    SessionSettings->bUseLobbiesVoiceChatIfAvailable = false;
    SessionSettings->bUsesStats = true; //Needed to keep track of player stats.
	SessionSettings->bIsLANMatch=true;

    // This custom attribute will be used in searches on GameClients. 
    SessionSettings->Settings.Add(KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));

    // Create session.
    UE_LOG(LogTemp, Log, TEXT("Creating session..."));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,TEXT("Creating session..."));

    if (!Session->CreateSession(0, SessionName, *SessionSettings))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
    	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Failed to create session"));
		Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
        CreateSessionDelegateHandle.Reset();
    }
}

void AMultiGameSession::HandleCreateSessionCompleted(FName MultiSessionName, bool bWasSuccessful) // Dedicated Server Only
{
	// Tutorial 3: This function is triggered via the callback we set in CreateSession once the session is created (or there is a failure to create)
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface(); // Retrieve the generic session interface. 

	// Nothing special here, simply log that the session is created.
	if (bWasSuccessful)
	{
		bSessionExists = true; 
		UE_LOG(LogTemp, Log, TEXT("Session: %s Created!"), *MultiSessionName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,FString::Printf(TEXT("Session: %s Created"), *MultiSessionName.ToString()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Failed to create session"));
	}

	// Clear our handle and reset the delegate. 
	Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
	CreateSessionDelegateHandle.Reset();
}
