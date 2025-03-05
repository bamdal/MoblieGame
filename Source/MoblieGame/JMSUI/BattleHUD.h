// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoblieGame/JMSInterface/BattleHUDInterface.h"
#include "BattleHUD.generated.h"

class UJMSButton;
class UListUI;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UBattleHUD : public UUserWidget, public IBattleHUDInterface
{
	GENERATED_BODY()
	

	UPROPERTY(meta = (BindWidget))
	UListUI* TextBlock_Timer;

	UPROPERTY(meta = (BindWidget))
	UListUI* TextBlock_Point;

	UPROPERTY(meta = (BindWidget))
	UJMSButton* ExitGame_Button;

	UFUNCTION()
	void OnClickExitGameButton();

	virtual void NativePreConstruct() override;

	// 남은 시간 카운트
	virtual void SetTextBlock_Timer_Implementation(int32 RemainingTime) override;

	// 현재 점수 / 최대 점수
	virtual void SetTextBlock_Point_Implementation(int32 CurrentPoint,int32 MaxPoint) override;
};
