// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiGameMode.h"
#include "MultiGameSession.h"
#include "MultiPlayerController.h"


AMultiGameMode::AMultiGameMode()
{
	GameSessionClass = AMultiGameSession::StaticClass();
	PlayerControllerClass = AMultiPlayerController::StaticClass();

}



void AMultiGameMode::CreateSession(FName KeyName, FString KeyValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,TEXT("Start CreateSession"));

	// 세션을 생성하기 위해 AMultiGameSession의 CreateSession 호출
	AMultiGameSession* MyMultiGameSession = Cast<AMultiGameSession>(GameSession);
	if(MyMultiGameSession)
	{
		MyMultiGameSession->CreateSession(KeyName, KeyValue);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,TEXT("Fuck NotGameSession"));

	}
	
}
