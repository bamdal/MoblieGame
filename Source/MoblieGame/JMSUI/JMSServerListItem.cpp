// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSServerListItem.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void UJMSServerListItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (B_JoinSession)
	{
		B_JoinSession->OnClicked.AddDynamic(this, &UJMSServerListItem::HandleButtonClicked);
	}

	if (T_AmountOfPlayerText)
	{
		T_AmountOfPlayerText->SetText(FText::FromString("Null"));
	}
}


void UJMSServerListItem::SetSessionName(const int32& NewSessionPort)
{
	PortNumber = NewSessionPort;
	T_SessionNameText->SetText(FText::AsNumber(NewSessionPort));
}

void UJMSServerListItem::SetAmountOfPlayerText(const FString Role)
{
	T_AmountOfPlayerText->SetText(FText::FromString(Role));
}

void UJMSServerListItem::HandleButtonClicked()
{
	if (OnJoinSessionDelegate.IsBound())
	{
		OnJoinSessionDelegate.Broadcast(this); // 자신을 넘겨줌
	}
}
