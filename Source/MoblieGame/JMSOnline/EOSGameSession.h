// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "EOSGameSession.generated.h"

/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AEOSGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AEOSGameSession();

	// Hardcoding the max number of players in a session. 
	const int MaxNumberOfPlayersInSession = 5;
	
	// Used to keep track if the session exists or not. 
	bool bSessionExists = false;

	FDelegateHandle CreateSessionDelegateHandle;
	
	// Function to create an EOS session. 
	void CreateSession(FName KeyName = "KeyName", FString KeyValue= "KeyValue");
	void HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful);
};
