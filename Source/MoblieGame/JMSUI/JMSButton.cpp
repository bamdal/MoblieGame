// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"




void UJMSButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	JMSTextBlock->SetText(SetTextBlock);
}

void UJMSButton::NativeConstruct()
{
	Super::NativeConstruct();

}

