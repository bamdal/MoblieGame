// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AMultiPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMultiPlayerController();

	FUniqueNetIdPtr UserId;

	
	// 서버 찾기후 자동 세션 진입
	UFUNCTION(BlueprintCallable)
	void Login();

	UFUNCTION(BlueprintCallable)
	void CreateSessionRequest();
protected:

	virtual void BeginPlay() override;


	
	// 서버에게 세션생성을 요청한다
	UFUNCTION(Server, Reliable)
	void ServerCreateSession();

	// 로그인 시도후 성공여부 판별
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);
	void OnFindSessionsComplete(bool bWasSuccessful);

	FDelegateHandle LoginDelegateHandle;
	FDelegateHandle SearchDelegateHandle;
	FDelegateHandle JoinSessionDelegateHandle;

	
	TSharedPtr<FOnlineSessionSearch> SessionSearchResult;


	
};
