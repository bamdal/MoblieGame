// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UListUI : public UUserWidget
{
	GENERATED_BODY()
	
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_TextBlock;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;


	UFUNCTION()
	void SetText(FText NewText);
	
protected:
	virtual void NativePreConstruct() override;
};
