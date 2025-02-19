// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMSStartUI.generated.h"

class UJMSButton;
/**
 * 
 */
UCLASS(Config=ServerConfig)
class MOBLIEGAME_API UJMSStartUI : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(Config)
	FString ServerUsername;

	UPROPERTY(Config)
	FString ServerIP;

	UPROPERTY(Config)
	FString ServerScriptPath;

	UPROPERTY(Config)
	FString ServerScriptName;

	UPROPERTY(Config)
	int32 BasePort;

	UPROPERTY(Config)
	int32 MaxServers;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	UJMSButton* CreateSessionButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	UJMSButton* JoinSessionButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(BindWidget))
	UJMSButton* ExitButton;

	UFUNCTION()
	void OnCreateSession();

	UFUNCTION()
	void OnJoinSession();

	UFUNCTION()
	void OnExitButton();
protected:

	virtual void NativeConstruct() override;
	
	
	
};
