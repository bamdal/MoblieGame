// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JMSDedicatedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UJMSDedicatedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void TravelSession();

	UFUNCTION(BlueprintCallable)
	void LocalTravelSession();

	
};
