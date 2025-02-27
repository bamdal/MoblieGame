// Fill out your copyright notice in the Description page of Project Settings.


#include "ListUI.h"

#include "Components/TextBlock.h"


void UListUI::SetText(FText NewText)
{
	T_TextBlock->SetText(NewText);
}

void UListUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	T_TextBlock->SetText(Text);
}

