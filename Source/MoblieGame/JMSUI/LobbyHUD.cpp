// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHUD.h"


void ULobbyHUD::SetTextToTextBlock_TotalPlayers(int32 TotalCountPlayers)
{ 
	FText PlayerCountText = FText::Format(FText::FromString(TEXT("현재 인원 : {0}")),FText::AsNumber(TotalCountPlayers));
	TextBlock_TotalPlayers->SetText(PlayerCountText);
}
