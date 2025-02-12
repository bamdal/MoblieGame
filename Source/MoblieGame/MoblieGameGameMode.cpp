// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoblieGameGameMode.h"
#include "MoblieGameCharacter.h"
#include "JMSOnline/EOSGameSession.h"
#include "JMSOnline/EOSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMoblieGameGameMode::AMoblieGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	if (PlayerPawnClassFinder.Class != NULL)
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	
 
	// 플레이어 컨트롤러 클래스를 내가 설정한 EOS Player로 설정
	PlayerControllerClass = AEOSPlayerController::StaticClass();
	GameSessionClass = AEOSGameSession::StaticClass(); // Tutorial 3: Setting the GameSession class to our custom one.
	
	// Tutorial 3: In a real game you may want to have a waiting room before sending players to the level. You can use seamless travel to do this and persist the EOS Session across levels. 
	// This is omitted in this tutorial to keep things simple. 
}
