// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMSServerListItem.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UJMSServerListItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* T_SessionNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* T_AmountOfPlayerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* B_JoinSession;

protected:
	virtual void NativeConstruct() override;


protected:
	UFUNCTION()
	void OnJoinSession();
public:
	void SetSessionName(const int32& NewSessionPort);
};
