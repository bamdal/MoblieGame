// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AEOSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AEOSPlayerController();

	
protected:

	virtual void BeginPlay() override;

	// 로그인 시도
	void Login();

	// 로그인 시도후 성공여부 판별
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorMessage);
	FDelegateHandle LoginDelegateHandle;
	


	
};
