// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMSButton.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UJMSButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* JMSButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* JMSTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SetTextBlock = FText::FromString("Set Text");

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
};
