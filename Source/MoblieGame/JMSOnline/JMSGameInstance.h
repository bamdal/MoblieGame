// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JMSGameInstance.generated.h"

class IOnlineSubsystem;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UJMSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UJMSGameInstance();
	
	
	virtual void Init() override;

	void Login();

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	
	UFUNCTION(BlueprintCallable)
	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	
protected:
	IOnlineSubsystem* OnlineSubsystem;

	bool bLoginSuccessful;
};
