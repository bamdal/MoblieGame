// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiPlayerController.h"

#include "MultiGameSession.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

AMultiPlayerController::AMultiPlayerController()
{
}

void AMultiPlayerController::BeginPlay()
{
	Super::BeginPlay();
}



void AMultiPlayerController::Login(bool Lan)
{
	// OSS 객체를 통해 인터페이스를 넣는과정 각각 서비스에 따라 다른 인터페이스 적용
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();



	// 세션 검색을 위한 객체 생성
	TSharedRef<FOnlineSessionSearch> SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->MaxSearchResults = 10; // 최대 검색 결과 개수
	SessionSearch->bIsLanQuery = Lan;
	
	/*
	FName KeyName = TEXT("MySessionKey");
	FString KeyValue = TEXT("MySessionValue");
	SessionSearch->QuerySettings.Set(KeyName, KeyValue, EOnlineComparisonOp::Equals);
	*/
	
	// 세션 검색 완료시 호출될 델리게이트 등록
	FOnFindSessionsCompleteDelegate SearchCompleteDelegate;
	SearchCompleteDelegate.BindUObject(this, &AMultiPlayerController::OnFindSessionsComplete);

	SearchDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(SearchCompleteDelegate);

	FName a = *FString::Printf(TEXT("Searching for sessions... (LAN: %s)"), SessionSearch->bIsLanQuery ? TEXT("TRUE") : TEXT("FALSE"));
	ServerLog(a);

	// 비동기 세션 검색 시작
	UE_LOG(LogTemp, Log, TEXT("Searching for available sessions..."));
	if (!Session->FindSessions(0, SessionSearch))
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessions failed to start!"));
		Session->ClearOnFindSessionsCompleteDelegate_Handle(SearchDelegateHandle);
		SearchDelegateHandle.Reset();
	}

	SessionSearchResult = SessionSearch;
	
	

}

void AMultiPlayerController::CreateSessionRequest(bool bIsLAN)
{
	if (HasAuthority())
	{
		ServerCreateSession_Implementation(bIsLAN);
		UE_LOG(LogTemp, Warning, TEXT("CreateSessionRequest!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("CreateSessionRequest"));

	}
	else
	{
		ServerCreateSession(bIsLAN);
		UE_LOG(LogTemp, Warning, TEXT("Client CreateSession Request!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("Client CreateSession Request!"));

	}
	
	
}

void AMultiPlayerController::FindSessionRequest(bool bIsLAN)
{
	if (HasAuthority())
	{
		ServerFindSession_Implementation(bIsLAN);
		UE_LOG(LogTemp, Warning, TEXT("FindSession Request!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("FindSessionRequest"));

	}
	else
	{
		ServerFindSession(bIsLAN);
		UE_LOG(LogTemp, Warning, TEXT("Client FindSession Request!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("Client FindSession Request!"));

	}
}


void AMultiPlayerController::DestroySession(FName SessionName)
{
	if (HasAuthority())
	{
		ServerDestroySession_Implementation(SessionName);
		UE_LOG(LogTemp, Warning, TEXT("CreateSessionRequest!"));

	}
	else
	{
		ServerDestroySession(SessionName);
		UE_LOG(LogTemp, Warning, TEXT("Client Request!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("Client Request!"));

	}
}

void AMultiPlayerController::ServerCreateSession_Implementation(bool Lan)
{
	UE_LOG(LogTemp, Log, TEXT("Server received session creation request from client"));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,   TEXT("Server received session creation request from client"));

	AMultiGameSession* GameSession = Cast<AMultiGameSession>(GetWorld()->GetAuthGameMode()->GameSession);
	if (GameSession)
	{
		UE_LOG(LogTemp, Log, TEXT("MySessionKey"));
		UE_LOG(LogTemp, Log, TEXT("MySessionValue"));
		
		GameSession->CreateSession(FName(TEXT("MySessionKey")), FString(TEXT("MySessionValue")),Lan);

	}
}


void AMultiPlayerController::ServerFindSession_Implementation(bool Lan)
{
	Login(Lan);	
}


void AMultiPlayerController::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	
	{
		Session->ClearOnFindSessionsCompleteDelegate_Handle(SearchDelegateHandle);
		SearchDelegateHandle.Reset();
	}
	
	if (!Session.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Session interface not valid!"));

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("Session interface not valid!"));

		return;
	}
	if (bWasSuccessful && SessionSearchResult.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Find Sessions Complete: Found %d Sessions"), SessionSearchResult->SearchResults.Num());

		for (int32 i = 0; i < SessionSearchResult->SearchResults.Num(); i++)
		{
			const FOnlineSessionSearchResult& Result = SessionSearchResult->SearchResults[i];

			if (Result.IsValid())
			{
				// 호스트 주소 정보 가져오기
				
				FString HostIP = *Result.Session.SessionInfo->GetSessionId().ToString();
				ServerLog(*HostIP);

			}
		}
	}
	else
	{
		ServerLog(TEXT("Find Sessions Failed or No Sessions Found"));
	}
	
	// 세션 검색 결과 확인
	if (!bWasSuccessful || SessionSearchResult->SearchResults.IsEmpty())
	{
		ServerLog(TEXT("No available sessions found!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No available sessions found!"));
		if (SessionSearchResult->SearchResults.IsEmpty())
		{
			ServerLog(TEXT("SearchResults.IsEmpty"));
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("SearchResults.IsEmpty"));
		}
		return;
	}

	// Presence를 사용하는 세션 필터링
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearchResult->SearchResults)
	{
		//if (SearchResult.Session.SessionSettings.bUsesPresence) // Presence를 사용하는 세션인지 확인
		{
			/*
			UE_LOG(LogTemp, Log, TEXT("Found session with Presence: %s"), *SearchResult.Session.OwningUserName);
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s"), *SearchResult.Session.OwningUserName));
			*/

			// 세션 참가 완료 시 호출될 델리게이트 등록
			FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
			JoinSessionCompleteDelegate.BindUObject(this, &AMultiPlayerController::OnJoinSessionComplete);
			JoinSessionDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			// 세션 참가 시도
			if (!Session->JoinSession(0, TEXT("GameSession"), SearchResult))
			{
				ServerLog(TEXT("JoinSession failed!"));
				
				Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
				JoinSessionDelegateHandle.Reset();
			}
			return;
		}
		
	}

	ServerLog(TEXT("No Presence-enabled sessions found!"));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No Presence-enabled sessions found!"));

}


void AMultiPlayerController::ServerDestroySession_Implementation(FName SessionName)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (!Session.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Session interface not valid!"));
		
		return;
	}

	if (SessionName==TEXT(""))
	{
		FName DefaultSessionNames[] = { NAME_GameSession, NAME_PartySession };

		for (FName SName : DefaultSessionNames)
		{
			if (Session->GetNamedSession(SName))
			{
				Session->DestroySession(SName);
				UE_LOG(LogTemp, Warning, TEXT("Destroyed Session: %s"), *SName.ToString());
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("All Sessions Destroyed"));
	
	}
	else
	{
		Session->DestroySession(SessionName);
		UE_LOG(LogTemp, Warning, TEXT("Destroyed Session: %s"), *SessionName.ToString());
	}

}



void AMultiPlayerController::ServerLog_Implementation(FName LogText)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *LogText.ToString());
}

void AMultiPlayerController::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
	if (JoinResult == EOnJoinSessionCompleteResult::Success)
	{
		ServerLog(TEXT("Successfully joined session!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Successfully joined session!"));

		FString ConnectionInfo;
		if (Session->GetResolvedConnectString(SessionName, ConnectionInfo) && !ConnectionInfo.IsEmpty())
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				PC->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
			}
		}
	}
	else
	{
		ServerLog(TEXT("Failed to join session!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Failed to join session"));

	}

	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}



