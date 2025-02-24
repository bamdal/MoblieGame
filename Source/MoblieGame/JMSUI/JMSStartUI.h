// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpFwd.h"
#include "Blueprint/UserWidget.h"
#include "JMSStartUI.generated.h"

class UVerticalBox;
class UWidgetSwitcher;
class UJMSServerListItem;
class FHttpModule;
class UJMSButton;
/**
 * 
 */
UCLASS(Config=ServerConfig)
class MOBLIEGAME_API UJMSStartUI : public UUserWidget
{
	GENERATED_BODY()

	//DefaultServerConfig


	UPROPERTY(Config)
	FString ServerUsername;

	UPROPERTY(Config)
	FString ServerIP;

	UPROPERTY(Config)
	FString ServerScriptPath;

	UPROPERTY(Config)
	FString ServerScriptName;

	UPROPERTY(Config)
	FString ServerResponsePort;

	UPROPERTY(Config)
	FString ServerCreatePath;

	UPROPERTY(Config)
	FString ServerListPath;

	UPROPERTY(Config)
	FString ServerDestroyPath;

	UPROPERTY(Config)
	int32 BasePort;

	UPROPERTY(Config)
	int32 MaxServers;

	UPROPERTY()
	FString BaseURL = FString("http://");

	// http
public:
	FHttpModule* Http;

	TSharedPtr<FJsonObject> RefJson;

	// 해당 URL로 HTTP요청 보내기
	UFUNCTION(BlueprintCallable)
	void HttpCall(const FString& InURL, const FString& InVerb);

	/**
	 * HTTP 요청을 보낸 후 서버로부터 응답을 받을 때 호출
	 * @param Request 요청을 나타내는 포인터로 정보를 확인하거나 요청을 추적하는 데 사용됩니다.
	 * @param Response 서버로부터 받은 응답 데이터를 이 객체에서 가져올 수 있습니다.
	 * @param bWasSuccessful 요청 성공 여부 반환
	 */
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// UI 바인딩
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UJMSButton* CreateSessionButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UJMSButton* FindSessionButton;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UJMSButton* ExitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UJMSButton* DestroySessionButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* ServerListVerticalBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UJMSButton* BackButton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox* ServerDestroyVerticalBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UJMSButton* BackButton2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UJMSServerListItem> ServerListButton;

	// 서버 접속용인지 삭제용인지 구분
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsJoin =true;


	FTimerHandle TimerHandle;
	
	UFUNCTION()
	void OnCreateSession();

	UFUNCTION()
	void OnFindSession();

	UFUNCTION()
	void OnJoinSession(UJMSServerListItem* ClickedItem);

	

	UFUNCTION()
	void OnDestroySequenceSession();

	UFUNCTION()
	void OnDestroySession(UJMSServerListItem* ClickedItem);

	UFUNCTION()
	void OnExitButton();

	UFUNCTION()
	void OnBackButton();

	UFUNCTION(Server, Reliable)
	void ServerLog(const FString& Message);

	

protected:
	virtual void NativeConstruct() override;
};
