// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSCreateSession.h"

#include "JMSButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MoblieGame/JMSOnline/MultiPlayerController.h"


void UJMSCreateSession::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnCreateSession);
	FindSessionButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnFindSession);
}

void UJMSCreateSession::OnCreateSession()
{
	AMultiPlayerController* PC = Cast<AMultiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSession!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("OnCreateSession"));
		PC->CreateSessionRequest();
	}
}

void UJMSCreateSession::OnFindSession()
{
	AMultiPlayerController* PC = Cast<AMultiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSession!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,  TEXT("OnCreateSession"));
		PC->Login();
	}
}
