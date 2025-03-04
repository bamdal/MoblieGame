// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "LobbyHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULobbyHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOBLIEGAME_API ILobbyHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTextToTextBlock_TotalPlayers(int32 TotalCountPlayers);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTextToTextBlock_ChaserStatus(int32 TotalChaserStatus);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTextToTextBlock_RunnerCount(int32 TotalRunnerCounts);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTextToTextBlock_CanStart(bool CanStart);

};
