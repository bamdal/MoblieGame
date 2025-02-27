// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSMultiGameState.h"

#include "PlayGameMode.h"
#include "Net/UnrealNetwork.h"


AJMSMultiGameState::AJMSMultiGameState()
{
}



void AJMSMultiGameState::UpdatePlayerCount()
{
	if (HasAuthority())
	{
		if (AuthGM == nullptr)
		{
			AuthGM = GetWorld()->GetAuthGameMode<APlayGameMode>();
		}
		
		CurrentPlayerCount = GetWorld()->GetAuthGameMode<APlayGameMode>()->GetCurrentPlayerCount();
		OnRep_CurrentPlayerCount();
	}
}

void AJMSMultiGameState::OnRep_CurrentPlayerCount()
{
	UE_LOG(LogTemp, Log, TEXT("현재 플레이어 수: %d"), CurrentPlayerCount);
	
}

void AJMSMultiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJMSMultiGameState, CurrentPlayerCount);
}

