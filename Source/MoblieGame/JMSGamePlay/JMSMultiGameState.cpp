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
		if (PC)
		{
			UE_LOG(LogTemp, Error, TEXT("현재 플레이어: %s"), *PC->GetName());
			PC->SetCountPlayerUI(CurrentPlayerCount);
			PC->SetChaserStatusUI(CurrentChaserCount);
			PC->SetRunnerCountUI(CurrentRunnerCount);
			PC->SetCanStartUI(CanPlay);	
		}
}

void AJMSMultiGameState::OnRep_CurrentChaserCount()
{
	UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 OnRep_CurrentChaserCount 호출됨 (현재 술래 수: %d)"), CurrentChaserCount);

	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetWorld()->GetFirstPlayerController());
	if (!HasAuthority())
		if (PC)
		{
			UE_LOG(LogTemp, Error, TEXT("현재 술래 수: %d"), CurrentChaserCount);
			PC->SetChaserStatusUI(CurrentChaserCount);
		}
}

void AJMSMultiGameState::OnRep_CurrentRunnerCount()
{
	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetWorld()->GetFirstPlayerController());
	if (!HasAuthority())
		if (PC)
		{
			UE_LOG(LogTemp, Error, TEXT("현재 도망자 수: %d"), CurrentRunnerCount);
			PC->SetRunnerCountUI(CurrentRunnerCount);
		}
}

void AJMSMultiGameState::OnRep_CanPlay()
{
	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetWorld()->GetFirstPlayerController());
	if (!HasAuthority())
		if (PC)
		{
			UE_LOG(LogTemp, Error, TEXT("시작가능: %d"), CanPlay);
			PC->SetCanStartUI(CanPlay);
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
