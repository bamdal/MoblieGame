// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiGameMode.generated.h"

class AMultiGameSession;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AMultiGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMultiGameMode();
	

	// 서버가 호출
	UFUNCTION(BlueprintCallable)
	void CreateSession(FName KeyName = "KeyName", FString KeyValue= "KeyValue");
	
};
