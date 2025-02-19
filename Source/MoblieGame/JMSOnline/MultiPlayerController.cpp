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

void AMultiPlayerController::Login()
{
	// OSS 객체를 통해 인터페이스를 넣는과정 각각 서비스에 따라 다른 인터페이스 적용
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();



	// 세션 검색을 위한 객체 생성
	TSharedRef<FOnlineSessionSearch> SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->MaxSearchResults = 10; // 최대 검색 결과 개수

	// 세션 검색 완료시 호출될 델리게이트 등록
	FOnFindSessionsCompleteDelegate SearchCompleteDelegate;
	SearchCompleteDelegate.BindUObject(this, &AMultiPlayerController::OnFindSessionsComplete);

	SearchDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(SearchCompleteDelegate);


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

void AMultiPlayerController::CreateSessionRequest()
{
	if (HasAuthority())
	{
		ServerCreateSession_Implementation();
		UE_LOG(LogTemp, Warning, TEXT("CreateSessionRequest!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("CreateSessionRequest"));

	}
	else
	{
		ServerCreateSession();
		UE_LOG(LogTemp, Warning, TEXT("Client Request!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("Client Request!"));

	}
	
	
}

void AMultiPlayerController::ServerCreateSession_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Server received session creation request from client"));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,   TEXT("Server received session creation request from client"));

	AMultiGameSession* GameSession = Cast<AMultiGameSession>(GetWorld()->GetAuthGameMode()->GameSession);
	if (GameSession)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,   TEXT("MySessionKey"));
		UE_LOG(LogTemp, Log, TEXT("MySessionKey"));

		GameSession->CreateSession(FName(TEXT("MySessionKey")), FString(TEXT("MySessionValue")));

	}
}

void AMultiPlayerController::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem ? Subsystem->GetSessionInterface() : nullptr;
	if (!Session.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Session interface not valid!"));

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("Session interface not valid!"));

		return;
	}

	// 세션 검색 결과 확인
	if (!bWasSuccessful || SessionSearchResult->SearchResults.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No available sessions found!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No available sessions found!"));

		return;
	}

	// Presence를 사용하는 세션 필터링
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearchResult->SearchResults)
	{
		if (SearchResult.Session.SessionSettings.bUsesPresence) // Presence를 사용하는 세션인지 확인
		{
			UE_LOG(LogTemp, Log, TEXT("Found session with Presence: %s"), *SearchResult.Session.OwningUserName);
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s"), *SearchResult.Session.OwningUserName));

			// 세션 참가 완료 시 호출될 델리게이트 등록
			FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
			JoinSessionCompleteDelegate.BindUObject(this, &AMultiPlayerController::OnJoinSessionComplete);
			JoinSessionDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			// 세션 참가 시도
			if (!Session->JoinSession(0, TEXT("GameSession"), SearchResult))
			{
				UE_LOG(LogTemp, Warning, TEXT("JoinSession failed!"));
				
				Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
				JoinSessionDelegateHandle.Reset();
			}
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No Presence-enabled sessions found!"));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No Presence-enabled sessions found!"));

}




void AMultiPlayerController::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
	if (JoinResult == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully joined session!"));
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
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Failed to join session"));

	}

	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}



