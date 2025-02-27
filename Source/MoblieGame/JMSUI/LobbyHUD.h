// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListUI.h"
#include "Blueprint/UserWidget.h"
#include "LobbyHUD.generated.h"

class UJMSButton;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API ULobbyHUD : public UUserWidget
{
	GENERATED_BODY()

	// widget binding
	UPROPERTY(Meta = (BindWidget))
	UListUI* TextBlock_TotalPlayers;
	UPROPERTY(Meta = (BindWidget))
	UListUI* TextBlock_ChaserStatus;
	UPROPERTY(Meta = (BindWidget))
	UListUI* TextBlock_RunnerCount;
	UPROPERTY(Meta = (BindWidget))
	UListUI* TextBlock_CanStart;
	UPROPERTY(Meta = (BindWidget))
	UJMSButton* GameStart_Button;
	UPROPERTY(Meta = (BindWidget))
	UJMSButton* ExitGame_Button;


	// GameState에서 GetCurrentPlayerCount() 가져오기
public:
	void SetTextToTextBlock_TotalPlayers(int32 TotalCountPlayers);
	
};
