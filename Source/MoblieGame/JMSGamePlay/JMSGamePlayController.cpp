// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSGamePlayController.h"

#include "MoblieGame/ETC/JMSEnum.h"
#include "MoblieGame/JMSCharacter/JMSCharBase.h"


void AJMSGamePlayController::Server_RequestResponseCharacter_Implementation(TSubclassOf<AJMSCharBase> Char, FVector Location)
{
	Multicast_UpdateResponseCharacter(Char, Location);
	UE_LOG(LogTemp, Warning, TEXT("AJMSDummy::Server_RequestResponseCharacter_Implementation"));
}

bool AJMSGamePlayController::Server_RequestResponseCharacter_Validate(TSubclassOf<AJMSCharBase> Char, FVector Location)
{
	return true;
}

void AJMSGamePlayController::Multicast_UpdateResponseCharacter_Implementation(TSubclassOf<AJMSCharBase> Char, FVector Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	APawn* OldNonePawn = GetPawn();
	if (OldNonePawn == nullptr)
		return;
	if (HasAuthority())
	{
		UnPossess();
		Possess(GetWorld()->SpawnActor<AJMSCharBase>(Char, Location, FRotator(0, 0, 0), SpawnParams));
		OldNonePawn->Destroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetPawn()->GetName());

	if (GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("New Pawn is possessed: %s"), *GetPawn()->GetName());
		if (GetPawn()->InputComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("InputComponent is valid"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InputComponent is missing!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No pawn is possessed"));
	}
}
