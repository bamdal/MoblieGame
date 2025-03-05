// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"

#include "JMSButton.h"
#include "ListUI.h"
#include "Components/Button.h"
#include "MoblieGame/JMSCharacter/JMSCharBase.h"



void UBattleHUD::OnClickExitGameButton()
{
	AJMSCharBase* P = Cast<AJMSCharBase>(GetOwningPlayerPawn());
	if (P)
	{
		
		P->ExitGame();
	}
}

void UBattleHUD::NativePreConstruct()
{
	Super::NativePreConstruct();

	ExitGame_Button->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnClickExitGameButton);

}

void UBattleHUD::SetTextBlock_Timer_Implementation(int32 RemainingTime)
{
	FText PlayerCountText = FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(RemainingTime));
	TextBlock_Timer->SetText(PlayerCountText);
}

void UBattleHUD::SetTextBlock_Point_Implementation(int32 CurrentPoint, int32 MaxPoint)
{
	FText PlayerCountText = FText::Format(FText::FromString(TEXT("{0} / {1}")), FText::AsNumber(CurrentPoint),FText::AsNumber(MaxPoint));
	TextBlock_Point->SetText(PlayerCountText);
}
