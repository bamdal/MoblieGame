// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "MultiGameSession.generated.h"

/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AMultiGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AMultiGameSession();

	// Hardcoding the max number of players in a session. 
	const int MaxNumberOfPlayersInSession = 10;
	
	// Used to keep track if the session exists or not. 
	bool bSessionExists = false;

	UFUNCTION(Server, Reliable)
	void ServerLog(FName LogText);
	
	FDelegateHandle CreateSessionDelegateHandle;
	
	// Function to create an Multi session.
	UFUNCTION(BlueprintCallable)
	void CreateSession(FName KeyName = "KeyName", FString KeyValue= "KeyValue",bool Lan=false);
	
	void HandleCreateSessionCompleted(FName MultiSessionName, bool bWasSuccessful);


	
};
