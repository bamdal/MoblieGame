// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoblieGameGameMode.h"
#include "MoblieGameCharacter.h"
#include "JMSOnline/EOSGameSession.h"
#include "JMSOnline/EOSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMoblieGameGameMode::AMoblieGameGameMode()
	: Super()
{

	// Tutorial 3: In a real game you may want to have a waiting room before sending players to the level. You can use seamless travel to do this and persist the EOS Session across levels. 
	// This is omitted in this tutorial to keep things simple. 
}
