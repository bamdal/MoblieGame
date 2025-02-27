// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSButton.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"




void UJMSButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (JMSTextBlock)
	JMSTextBlock->SetText(SetTextBlock);

	FSlateFontInfo FontInfo = JMSTextBlock->GetFont();
	FontInfo.Size = TextSize;
	JMSTextBlock->SetFont(FontInfo);
	
	SizeBox->SetWidthOverride(SizeX);
	SizeBox->SetHeightOverride(SizeY);
}

void UJMSButton::NativeConstruct()
{
	Super::NativeConstruct();

}

