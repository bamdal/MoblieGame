// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSStartUI.h"

#include "JMSButton.h"
#include "Components/Button.h"


void UJMSStartUI::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnCreateSession);
	JoinSessionButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnJoinSession);
	ExitButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnExitButton);

	UE_LOG(LogTemp, Display, TEXT("ServerUsername: %s"), *ServerUsername);
	UE_LOG(LogTemp, Display, TEXT("ServerIP: %s"), *ServerIP);
	UE_LOG(LogTemp, Display, TEXT("ServerScriptPath: %s"), *ServerScriptPath);
	UE_LOG(LogTemp, Display, TEXT("ServerScriptName: %s"), *ServerScriptName);
	UE_LOG(LogTemp, Display, TEXT("BasePort: %d"), BasePort);

}


void UJMSStartUI::OnCreateSession()
{
	UE_LOG(LogTemp, Display, TEXT("OnCreateSession"));
}

void UJMSStartUI::OnJoinSession()
{
	UE_LOG(LogTemp, Display, TEXT("OnJoinSession"));
}

void UJMSStartUI::OnExitButton()
{
	UE_LOG(LogTemp, Display, TEXT("OnExitButton"));
}
