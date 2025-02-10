// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"


UJMSGameInstance::UJMSGameInstance()
{
	bLoginSuccessful = false;
}

void UJMSGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = Online::GetSubsystem(GetWorld());
	Login();
}

void UJMSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;

			Credentials.Type = TEXT("developer");  // EOS 로그인 타입 지정
			Credentials.Id = TEXT("127.0.0.1:8081");  // 공백 유지
			Credentials.Token = TEXT("JMSTestUser");  // 공백 유지

			Identity->OnLoginCompleteDelegates->AddUObject(this,&ThisClass::OnLoginComplete);
			Identity->Login(0,Credentials);
		}
	}
}

void UJMSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	bLoginSuccessful = bWasSuccessful;
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			if (bWasSuccessful)
			{
				UE_LOG(LogTemp, Log, TEXT("UJMSGameInstance::OnLoginComplete - Login CallBack successfully"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("UJMSGameInstance::OnLoginComplete - EOS Login Failed"));
			}

			// 실패시 델리게이트 삭제후 재설정하는 함수
			Identity->ClearOnLoginCompleteDelegates(LocalUserNum,this);
		}
	}
}

void UJMSGameInstance::CreateSession()
{
	if (bLoginSuccessful)
	{
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
			if (SessionPtr)
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = true;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;

				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this,&ThisClass::OnCreateSessionComplete);
				SessionPtr->CreateSession(0,TEXT("Test Session"),SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("UJMSGameInstance::CreateSession CanNot Create Session - Login Fail"));
	}
}

void UJMSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{

	UE_LOG(LogTemp,Warning,TEXT("UJMSGameInstance::OnCreateSessionComplete Succeed : %d"),bWasSuccessful);
	/*if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
		if (SessionPtr)
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		}
	}*/
}

