// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSServerListItem.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void UJMSServerListItem::NativeConstruct()
{
	Super::NativeConstruct();

	if (B_JoinSession)
		B_JoinSession->OnClicked.AddDynamic(this, &ThisClass::UJMSServerListItem::OnJoinSession);
}

void UJMSServerListItem::OnJoinSession()
{
	
}

void UJMSServerListItem::SetSessionName(const int32& NewSessionPort)
{
	T_SessionNameText->SetText(FText::AsNumber(NewSessionPort));
}


