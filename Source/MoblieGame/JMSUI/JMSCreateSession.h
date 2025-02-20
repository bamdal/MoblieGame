// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMSCreateSession.generated.h"

class UCheckBox;
class UJMSButton;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UJMSCreateSession : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta = (BindWidget))
	UJMSButton* CreateSessionButton;
	
	UPROPERTY(Meta = (BindWidget))
	UJMSButton* FindSessionButton;

	UPROPERTY(Meta = (BindWidget))
	UJMSButton* DestroySessionButton;

	UPROPERTY(Meta = (BindWidget))
	UCheckBox* LANCheckBox;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnCreateSession();

	UFUNCTION()
	void OnFindSession();

	UFUNCTION()
	void OnDestroySession();
	
	
};
