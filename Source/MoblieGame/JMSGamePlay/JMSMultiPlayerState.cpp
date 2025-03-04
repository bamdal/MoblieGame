// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSMultiPlayerState.h"

#include "JMSMultiGameState.h"
#include "Net/UnrealNetwork.h"


AJMSMultiPlayerState::AJMSMultiPlayerState()
{
	bReplicates = true;
}

void AJMSMultiPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		MyGameState = Cast<AJMSMultiGameState>(GetWorld()->GetGameState());
		if (MyGameState == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AJMSMultiPlayerState::BeginPlay() failed"));
		}
	}
}

EDummyState AJMSMultiPlayerState::GetPlayerCharacterRoleState() const
{
	return PlayerCharacterRoleState;
}

void AJMSMultiPlayerState::Server_UpdateChaserCount_Implementation(int32 NewValue)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		AJMSMultiGameState* GS = Cast<AJMSMultiGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CurrentChaserCount = NewValue;
		}
	}
}

void AJMSMultiPlayerState::Server_UpdateRunnerCount_Implementation(int32 NewValue)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		AJMSMultiGameState* GS = Cast<AJMSMultiGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CurrentRunnerCount = NewValue;
		}
	}
}

void AJMSMultiPlayerState::Server_UpdateCanPlay_Implementation(bool NewValue)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		AJMSMultiGameState* GS = Cast<AJMSMultiGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CanPlay = NewValue;
		}
	}
}

void AJMSMultiPlayerState::SetPlayerCharacterRoleState_Implementation(EDummyState NewPlayerCharacterRoleState)
{
	if (PlayerCharacterRoleState == NewPlayerCharacterRoleState)
		return;

	UE_LOG(LogTemp, Error, TEXT("Role 변경"));


	// 술래가 술래가 아니게 되었을때
	if (PlayerCharacterRoleState == EDummyState::Chaser && NewPlayerCharacterRoleState != EDummyState::Chaser)
	{
		// 술래 해제 알림
		if (MyGameState)
		{
			MyGameState->CurrentChaserCount = 0;
			MyGameState->OnRep_CurrentChaserCount();
			MyGameState->CanPlay = (MyGameState->CurrentPlayerCount == MyGameState->CurrentChaserCount + MyGameState->CurrentRunnerCount) &&
				MyGameState->CurrentChaserCount;
			MyGameState->OnRep_CanPlay();
		}
	}

	// 도망자인데 역할 변경 요청
	if (PlayerCharacterRoleState != EDummyState::Chaser && PlayerCharacterRoleState != EDummyState::Runner_None)
	{
		//도망자 알림
		if (MyGameState)
		{
			MyGameState->CurrentRunnerCount--;
			MyGameState->OnRep_CurrentRunnerCount();
			MyGameState->CanPlay = (MyGameState->CurrentPlayerCount == MyGameState->CurrentChaserCount + MyGameState->CurrentRunnerCount) &&
				MyGameState->CurrentChaserCount;
			MyGameState->OnRep_CanPlay();
		}
	}

	// 서버에 LogOut이 호출되었을경우
	if (NewPlayerCharacterRoleState == EDummyState::Runner_None)
		return;


	// 역할정보 업데이트
	this->PlayerCharacterRoleState = NewPlayerCharacterRoleState;

	if (PlayerCharacterRoleState == EDummyState::Chaser)
	{
		// 술래 알림
		if (MyGameState)
		{
			UE_LOG(LogTemp, Error, TEXT("술래"));
			MyGameState->CurrentChaserCount = 1;
			MyGameState->OnRep_CurrentChaserCount();
			MyGameState->CanPlay = (MyGameState->CurrentPlayerCount == MyGameState->CurrentChaserCount + MyGameState->CurrentRunnerCount) &&
				MyGameState->CurrentChaserCount;
			MyGameState->OnRep_CanPlay();
		}
	}
	else
	{
		//도망자 알림
		if (MyGameState)
		{
			MyGameState->CurrentRunnerCount++;
			MyGameState->OnRep_CurrentRunnerCount();
			MyGameState->CanPlay = (MyGameState->CurrentPlayerCount == MyGameState->CurrentChaserCount + MyGameState->CurrentRunnerCount) &&
				MyGameState->CurrentChaserCount;
			MyGameState->OnRep_CanPlay();
		}
	}
	if (MyGameState)
	{
		Server_UpdateChaserCount(MyGameState->CurrentChaserCount);
		Server_UpdateRunnerCount(MyGameState->CurrentRunnerCount);
		Server_UpdateCanPlay(MyGameState->CanPlay);
	}
}


void AJMSMultiPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJMSMultiPlayerState, PlayerCharacterRoleState);
}
