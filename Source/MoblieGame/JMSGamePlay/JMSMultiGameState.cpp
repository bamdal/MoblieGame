// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSMultiGameState.h"

#include "JMSGamePlayController.h"
#include "PlayGameMode.h"
#include "GameFramework/HUD.h"
#include "Net/UnrealNetwork.h"


AJMSMultiGameState::AJMSMultiGameState()
{
	bReplicates = true;
}


void AJMSMultiGameState::UpdatePlayerCount()
{
	if (HasAuthority())
	{
		if (AuthGM == nullptr)
		{
			AuthGM = GetWorld()->GetAuthGameMode<APlayGameMode>();
		}

		CurrentPlayerCount = AuthGM->GetCurrentPlayerCount();
	
	}
}


void AJMSMultiGameState::OnRep_CurrentPlayerCount()
{
	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetWorld()->GetFirstPlayerController());
	if (!HasAuthority())
	{
		if (PC)
		{
			PC->SetCountPlayerUI(CurrentPlayerCount);
			PC->SetChaserStatusUI(CurrentChaserCount);
			PC->SetRunnerCountUI(CurrentRunnerCount);
			PC->SetCanStartUI(CanPlay);	
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnRep_CurrentPlayerCount Not PC"));
		}
	}

}

void AJMSMultiGameState::OnRep_CurrentChaserCount()
{

	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetWorld()->GetFirstPlayerController());
	if (!HasAuthority())
	{
		if (PC)
		{
			
			PC->SetChaserStatusUI(CurrentChaserCount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnRep_CurrentChaserCount Not PC"));
		}
	}

}

void AJMSMultiGameState::OnRep_CurrentRunnerCount()
{
	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetWorld()->GetFirstPlayerController());
	if (!HasAuthority())
	{
		if (PC)
		{
			
			PC->SetRunnerCountUI(CurrentRunnerCount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnRep_CurrentRunnerCount Not PC"));
		}
	}

}

void AJMSMultiGameState::OnRep_CanPlay()
{
	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetWorld()->GetFirstPlayerController());
	if (!HasAuthority())
	{
		if (PC)
		{
			PC->SetCanStartUI(CanPlay);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OnRep_CanPlay Not PC"));
		}
	}

}

void AJMSMultiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJMSMultiGameState, CurrentPlayerCount);
	DOREPLIFETIME(AJMSMultiGameState, CurrentChaserCount);
	DOREPLIFETIME(AJMSMultiGameState, CurrentRunnerCount);
	DOREPLIFETIME(AJMSMultiGameState, CanPlay);
}
