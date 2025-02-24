// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMSServerListItem.generated.h"

class UButton;
class UTextBlock;
/**
 * 포트 번호 저장, 본인이 눌린것 리턴
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinSessionClicked, UJMSServerListItem*, ClickedItem);
UCLASS()
class MOBLIEGAME_API UJMSServerListItem : public UUserWidget
{
	GENERATED_BODY()

private:
	int32 PortNumber;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* T_SessionNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* T_AmountOfPlayerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* B_JoinSession;

	UPROPERTY(BlueprintAssignable)
	FOnJoinSessionClicked OnJoinSessionDelegate;

	UFUNCTION()
	void HandleButtonClicked();

protected:
	virtual void NativeConstruct() override;

public:
	void SetSessionName(const int32& NewSessionPort);

	void SetAmountOfPlayerText(const FString Role);
	
	int32 GetSessionPort() const {return PortNumber;}
};
