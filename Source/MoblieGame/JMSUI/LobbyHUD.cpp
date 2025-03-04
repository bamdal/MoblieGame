// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHUD.h"

#include "JMSButton.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


void ULobbyHUD::SetTextToTextBlock_TotalPlayers_Implementation(int32 TotalCountPlayers)
{
	FText PlayerCountText = FText::Format(FText::FromString(TEXT("현재 인원 : {0}")), FText::AsNumber(TotalCountPlayers));
	TextBlock_TotalPlayers->SetText(PlayerCountText);
}

void ULobbyHUD::SetTextToTextBlock_ChaserStatus_Implementation(int32 TotalChaserStatus)
{
	FText PlayerCountText = FText::Format(FText::FromString(TEXT("현재 술래 : {0}")), FText::AsNumber(TotalChaserStatus));
	TextBlock_ChaserStatus->SetText(PlayerCountText);
}

void ULobbyHUD::SetTextToTextBlock_RunnerCount_Implementation(int32 RunnerCount)
{
	FText PlayerCountText = FText::Format(FText::FromString(TEXT("현재 도망자 : {0}")), FText::AsNumber(RunnerCount));
	TextBlock_RunnerCount->SetText(PlayerCountText);
}

void ULobbyHUD::SetTextToTextBlock_CanStart_Implementation(bool CanStart)
{
	FText PlayerCountText = FText::FromString(TEXT("시작 불가능"));
	if (CanStart)
	{
		PlayerCountText = FText::FromString(TEXT("시작 가능"));
	}
	TextBlock_CanStart->SetText(PlayerCountText);

}


void ULobbyHUD::OnClickExitButton()
{
	// 게임 종료 기능
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void ULobbyHUD::OnClickStartButton()
{
	// 게임 시작 기능
	UE_LOG(LogTemp, Warning, TEXT("OnClickStartButton"));
}

void ULobbyHUD::NativePreConstruct()
{
	Super::NativePreConstruct();

#if PLATFORM_WINDOWS
	ExitGame_Button->JMSTextBlock->SetText(FText::FromString(TEXT("Exit(ESC)")));
	GameStart_Button->JMSTextBlock->SetText(FText::FromString(TEXT("Start(P)")));
#elif PLATFORM_ANDROID
	GameStart_Button->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnClickStartButton);
	ExitGame_Button->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnClickExitButton);
	ExitGame_Button->JMSTextBlock->SetText(FText::FromString(TEXT("Exit")));
	
#endif
}
