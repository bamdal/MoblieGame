// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayGameMode.generated.h"

class AJMSDummyButton;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;


	void AllowCharacterSelection(APlayerController* NewPlayer);
	
};
