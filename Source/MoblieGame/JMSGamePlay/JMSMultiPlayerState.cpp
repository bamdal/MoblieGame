// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSMultiPlayerState.h"

#include "Net/UnrealNetwork.h"


AJMSMultiPlayerState::AJMSMultiPlayerState()
{
}

void AJMSMultiPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJMSMultiPlayerState,PlayerCharacterRoleState);
}
