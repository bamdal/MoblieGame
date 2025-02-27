// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSDummy.h"

#include "MoblieGame/ETC/JMSEnum.h"
#include "MoblieGame/JMSCharacter/JMSCharBase.h"
#include "MoblieGame/JMSGamePlay/JMSGamePlayController.h"


// Sets default values
AJMSDummy::AJMSDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJMSDummy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AJMSDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


bool AJMSDummy::DummyButtonClick_Implementation(AJMSCharBase* Char)
{
	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(Char->GetController());
	if (PC)
	{
		uint8 Index = static_cast<uint8>(DummyState);

		PC->Server_RequestResponseCharacter(PlayerCharacters[Index],Char->GetActorLocation());

	}
	return DummyState == EDummyState::Chaser;
}

EDummyState AJMSDummy::GetDummyCharacterState_Implementation()
{
	return DummyState;
}
