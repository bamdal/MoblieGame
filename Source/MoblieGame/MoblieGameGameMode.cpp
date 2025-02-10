// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoblieGameGameMode.h"
#include "MoblieGameCharacter.h"
#include "JMSOnline/EOSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMoblieGameGameMode::AMoblieGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// 플레이어 컨트롤러 클래스를 내가 설정한 EOS Player로 설정
	PlayerControllerClass = AEOSPlayerController::StaticClass();
}
