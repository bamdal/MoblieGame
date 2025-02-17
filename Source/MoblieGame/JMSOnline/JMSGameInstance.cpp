// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"


const FName JMSSessionName = TEXT("Test Session");

UJMSGameInstance::UJMSGameInstance()
{
	bIsLoginIn = false;
}

void UJMSGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = Online::GetSubsystem(GetWorld());

}

void UJMSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;

			Credentials.Type = TEXT("developer"); // EOS 로그인 타입 지정
			Credentials.Id = TEXT("127.0.0.1:8081"); // 공백 유지
			Credentials.Token = TEXT("JMSTestUser"); // 공백 유지

			/*Credentials.Type = TEXT("accountportal");  // EOS 로그인 타입 지정
			Credentials.Id = TEXT("");  // 공백 유지
			Credentials.Token = TEXT("");  // 공백 유지*/

			Identity->OnLoginCompleteDelegates->AddUObject(this, &ThisClass::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UJMSGameInstance::Login2()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;

			Credentials.Type = TEXT("developer"); // EOS 로그인 타입 지정
			Credentials.Id = TEXT("127.0.0.1:8081"); // 공백 유지
			Credentials.Token = TEXT("JMSTestUser2"); // 공백 유지

			/*Credentials.Type = TEXT("accountportal");  // EOS 로그인 타입 지정
			Credentials.Id = TEXT("");  // 공백 유지
			Credentials.Token = TEXT("");  // 공백 유지*/

			Identity->OnLoginCompleteDelegates->AddUObject(this, &ThisClass::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UJMSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	bIsLoginIn = bWasSuccessful;
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
			Identity->ClearOnLoginCompleteDelegates(LocalUserNum, this);
		}
	}
}

void UJMSGameInstance::CreateSession()
{
	if (bIsLoginIn)
	{
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
			if (SessionPtr)
			{
				IOnlineIdentityPtr IdentityPtr = OnlineSubsystem->GetIdentityInterface();
				if (IdentityPtr.IsValid())
				{
					TSharedPtr<const FUniqueNetId> UserId = IdentityPtr->GetUniquePlayerId(0);
					if (!UserId.IsValid())
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to get Unique Player ID! User may not be logged in."));
						return;
					}
				}


				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;

				SessionSettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineDataAdvertisementType::ViaOnlineService);

				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);
				SessionPtr->CreateSession(0, JMSSessionName, SessionSettings);
			}


			/*
			IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
			IOnlineSessionPtr Session = Subsystem->GetSessionInterface(); // Retrieve the generic session interface. 


			// Set session settings 
			TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
			SessionSettings->NumPublicConnections = 5; //We will test our sessions with 2 players to keep things simple
			SessionSettings->bShouldAdvertise = true;
			//This creates a public match and will be searchable. This will set the session as joinable via presence. 
			SessionSettings->bUsesPresence = false; //No presence on dedicated server. This requires a local user.
			SessionSettings->bAllowJoinViaPresence = false; // superset by bShouldAdvertise and will be true on the backend
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false; // superset by bShouldAdvertise and will be true on the backend
			SessionSettings->bAllowInvites = false; //Allow inviting players into session. This requires presence and a local user. 
			SessionSettings->bAllowJoinInProgress = false; //Once the session is started, no one can join.
			SessionSettings->bIsDedicated = false; //Session created on dedicated server.
			SessionSettings->bUseLobbiesIfAvailable = false; //This is an EOS Session not an EOS Lobby as they aren't supported on Dedicated Servers.
			SessionSettings->bUseLobbiesVoiceChatIfAvailable = false;
			SessionSettings->bUsesStats = true; //Needed to keep track of player stats.
			SessionSettings->bIsLANMatch = false;

			// This custom attribute will be used in searches on GameClients. 
			SessionSettings->Settings.Add(SEARCH_KEYWORDS, FOnlineSessionSetting(FString("YTTutorialLobby"), EOnlineDataAdvertisementType::ViaOnlineService));
			


			// Create session.
			UE_LOG(LogTemp, Log, TEXT("Creating session..."));

			if (!Session->CreateSession(0, TEXT("TestSession"), *SessionSettings))
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Session created!"));

			}*/
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJMSGameInstance::CreateSession CanNot Create Session - Login Fail"));
	}
}

void UJMSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("UJMSGameInstance::OnCreateSessionComplete Succeed : %d"), bWasSuccessful);

	if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
		if (SessionPtr)
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		}
	}
}

void UJMSGameInstance::DestroySession()
{
	if (bIsLoginIn)
	{
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
			if (SessionPtr)
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionComplete);
				SessionPtr->DestroySession(JMSSessionName);
			}
		}
	}
}

void UJMSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
		if (SessionPtr)
		{
			SessionPtr->ClearOnDestroySessionCompleteDelegates(this);
		}
	}
}

void UJMSGameInstance::GetAllFriends()
{
	if (bIsLoginIn)
	{
		if (OnlineSubsystem)
		{
			IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface();
			if (FriendsPtr)
			{
				FriendsPtr->ReadFriendsList(0, FString(""),
				                            FOnReadFriendsListComplete::CreateUObject(this, &UJMSGameInstance::OnReadFriendsListComplete));
			}
		}
	}
}

void UJMSGameInstance::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	if (bWasSuccessful)
	{
		if (OnlineSubsystem)
		{
			IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface();
			if (FriendsPtr)
			{
				TArray<TSharedRef<FOnlineFriend>> FriendsList;
				if (FriendsPtr->GetFriendsList(0, ListName, FriendsList))
				{
					for (TSharedRef<FOnlineFriend> OnlineFriend : FriendsList)
					{
						FString FriendName = OnlineFriend.Get().GetRealName();
						UE_LOG(LogTemp, Warning, TEXT("Friend Name: %s"), *FriendName);
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("OnReadFriendsListComplete FriendsList is Fail!!"));
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("OnReadFriendsListComplete is Success At Getting All FriendsList!!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnReadFriendsListComplete bWasSuccessful is Fail!!"));
	}
}

void UJMSGameInstance::ShowInviteUI()
{
	if (bIsLoginIn)
	{
		if (OnlineSubsystem)
		{
			IOnlineExternalUIPtr ExUIPtr = OnlineSubsystem->GetExternalUIInterface();
			if (ExUIPtr)
			{
				ExUIPtr->ShowInviteUI(0, JMSSessionName);
			}
		}
	}
}

void UJMSGameInstance::ShowFriendsUI()
{
	if (bIsLoginIn)
	{
		if (OnlineSubsystem)
		{
			IOnlineExternalUIPtr ExUIPtr = OnlineSubsystem->GetExternalUIInterface();
			if (ExUIPtr)
			{
				ExUIPtr->ShowFriendsUI(0);
			}
		}
	}
}

void UJMSGameInstance::FindSessions()
{
	if (bIsLoginIn)
	{
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
			if (SessionPtr)
			{
				// 메모리 관리를 위한 스마트 포인터 사용 그냥 new쓰면 위험하니까
				SearchSetting = MakeShareable(new FOnlineSessionSearch);

				// 세션 검색시 찾게될 설정
				SearchSetting->MaxSearchResults = 8000;
				SearchSetting->QuerySettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineComparisonOp::Equals);
				SearchSetting->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UJMSGameInstance::OnFindSessionsComplete);
				SessionPtr->FindSessions(0, SearchSetting.ToSharedRef());
			}
		}
	}
}

void UJMSGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete bWasSuccessful is %d!!"), bWasSuccessful);
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find %d Lobbies"), SearchSetting->SearchResults.Num());
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
			if (SessionPtr)
			{
				if (SearchSetting->SearchResults.Num() > 0)
				{
					SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionComplete);
					SessionPtr->JoinSession(0, JMSSessionName, SearchSetting->SearchResults[0]);
				}
			}
		}
	}

	if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
		if (SessionPtr)
		{
			SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		}
	}
}

void UJMSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (JoinResult == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Success!!"));
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Failed!!"));
	}
	
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
		if (SessionPtr)
		{
			FString ConnectionInfo = FString();
			SessionPtr->GetResolvedConnectString(SessionName,ConnectionInfo);
			if (!ConnectionInfo.IsEmpty())
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0))
				{
					PC->ClientTravel(ConnectionInfo,TRAVEL_Absolute);
				}
			}
			SessionPtr->ClearOnJoinSessionCompleteDelegates(this);
		}
	}
}
