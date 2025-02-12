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

	// 로그인 성공시 세션 만들기
	UFUNCTION(BlueprintCallable)
	void CreateSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// 현재 세션 파괴
	UFUNCTION(BlueprintCallable)
	void DestroySession();

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// 스팀 친구 목록 가져오기
	UFUNCTION(BlueprintCallable)
	void GetAllFriends();

	void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	// 친구 UI 표시
	UFUNCTION(BlueprintCallable)
	void ShowUI();

protected:
	// EOS 연결
	IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoginIn;
};
