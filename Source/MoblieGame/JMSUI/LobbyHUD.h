// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListUI.h"
#include "Blueprint/UserWidget.h"
#include "MoblieGame/JMSInterface/LobbyHUDInterface.h"
#include "LobbyHUD.generated.h"

class UJMSButton;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API ULobbyHUD : public UUserWidget, public ILobbyHUDInterface
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
	virtual void SetTextToTextBlock_TotalPlayers_Implementation(int32 TotalCountPlayers) override;
	virtual void SetTextToTextBlock_ChaserStatus_Implementation(int32 TotalChaserStatus) override;
	virtual void SetTextToTextBlock_RunnerCount_Implementation(int32 RunnerCount) override;
	virtual void SetTextToTextBlock_CanStart_Implementation(bool CanStart) override;
	
protected:
	UFUNCTION()
	void OnClickExitButton();

	UFUNCTION()
	void OnClickStartButton();
	virtual void NativePreConstruct() override;
};
