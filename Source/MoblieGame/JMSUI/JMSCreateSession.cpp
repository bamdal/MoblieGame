// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSCreateSession.h"

#include "JMSButton.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"
#include "MoblieGame/JMSOnline/MultiPlayerController.h"


void UJMSCreateSession::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreateSessionButton)
		CreateSessionButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnCreateSession);
	if (FindSessionButton)
		FindSessionButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnFindSession);
	if (DestroySessionButton)
		DestroySessionButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnDestroySession);
}

void UJMSCreateSession::OnCreateSession()
{
	AMultiPlayerController* PC = Cast<AMultiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSession!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("OnCreateSession"));
		bool i = LANCheckBox->IsChecked();
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::FromInt(i));
		PC->CreateSessionRequest(LANCheckBox->IsChecked());
	}
}

void UJMSCreateSession::OnFindSession()
{
	AMultiPlayerController* PC = Cast<AMultiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSession!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("OnFindSession"));
		PC->Login(LANCheckBox->IsChecked());
	}
}

void UJMSCreateSession::OnDestroySession()
{
	AMultiPlayerController* PC = Cast<AMultiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySession!"));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("DestroySession"));
		PC->DestroySession();
	}
}
