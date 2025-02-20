// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSStartUI.h"

#include "HttpModule.h"
#include "JMSButton.h"
#include "JMSServerListItem.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Components/Button.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"


void UJMSStartUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreateSessionButton)
		CreateSessionButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnCreateSession);
	if (FindSessionButton)
		FindSessionButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnFindSession);
	if (DestroySessionButton)
		DestroySessionButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnDestroySession);
	if (ExitButton)
		ExitButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButton);
	if (BackButton)
		BackButton->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButton);
	if (BackButton2)
		BackButton2->JMSButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButton);
	

	// FHttpModule 가져와 저장
	Http = &FHttpModule::Get();
	
}

void UJMSStartUI::HttpCall(const FString& InURL, const FString& InVerb)
{
	// 요청 객체 생성
	TSharedPtr<IHttpRequest> Request = Http->CreateRequest();

	// 요청완료 델리게이트 바인딩
	Request->OnProcessRequestComplete().BindUObject(this,&UJMSStartUI::OnResponseReceived);
	
	// URL과 Get,Post 요청 메서드 설정, 요청 헤더 설정
	Request->SetURL(InURL);
	Request->SetVerb(InVerb);
	Request->SetHeader("Content-Type", "application/json");

	// 요청 전송
	Request->ProcessRequest();

	ServerLog(FString::Printf(TEXT("Http Call %s"),*InURL));
}

void UJMSStartUI::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ServerLog(TEXT("Http Request Response Failed"));
		return;
	}
	// 응답 상태 코드를 가져온다.
	int32 StatusCode = Response->GetResponseCode();

	// 응답 본문을 문자열로 가져옴
	FString ResponseContent = Response->GetContentAsString();

	// 요청이 완료되었음을 출력
	ServerLog(FString::Printf(TEXT("HTTP request completed. Status Code: %d, URL: %s"), StatusCode, *Request->GetURL()));
	ServerLog(FString::Printf(TEXT("Response Content: %s"), *ResponseContent));

	// Json 데이터를 파싱하기 위한 리더 객체
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
	
	// Reader를 사용해 문자열을 파싱하고, 이를 FJsonObject로 변환한다.
	FJsonSerializer::Deserialize(JsonReader,RefJson);

	// CreateSession
	if (RefJson->HasField(TEXT("port")))
	{
		ServerLog(FString::Printf(TEXT("Server: %s"), *RefJson->GetStringField(TEXT("port"))));
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&UJMSStartUI::OnFindSession,3.0f,false);
	}

	// FindSession
	int32 LastIndex = RefJson->GetArrayField(TEXT("servers")).Num() -1;
	if (RefJson->HasField(TEXT("servers")) && LastIndex > -1)
	{
		if (ServerListVerticalBox)
			ServerListVerticalBox->ClearChildren(); // 기존 목록 초기화
		
		int i = 0;
		for (TSharedPtr<FJsonValue> Content : RefJson->GetArrayField(TEXT("servers")))
		{
			i++;
			if (Content.IsValid() &&  Content->Type == EJson::Number)
			{
				const int32 FindPort = static_cast<int>(Content->AsNumber());
				ServerLog(FString::Printf(TEXT("Server: %d"),FindPort));
				if (ServerListButton)
				{
					// UJMSServerListItem 위젯 생성
					UJMSServerListItem* NewServerListItem = CreateWidget<UJMSServerListItem>(this, ServerListButton);
					if (!NewServerListItem) continue;
					NewServerListItem->SetSessionName(FindPort);
					ServerListVerticalBox->AddChild(NewServerListItem);
				}
			
			}
			if (i >=LastIndex && WidgetSwitcher)
			{
				// 서버 조인버튼으로 이동
				WidgetSwitcher->SetActiveWidgetIndex(2);
			}		
		}
	}
	else if (RefJson->HasField(TEXT("servers")))
	{
		// 실패시 원래 화면으로 복귀
		if (WidgetSwitcher)
			WidgetSwitcher->SetActiveWidgetIndex(2);
	}

	// DestroySession
	if (RefJson->HasField(TEXT("message")))
		ServerLog(FString::Printf(TEXT("Server: %s"), *RefJson->GetStringField(TEXT("message"))));

}

void UJMSStartUI::OnCreateSession()
{
	if (WidgetSwitcher)
		WidgetSwitcher->SetActiveWidgetIndex(1);
	// 동시에 누르면 같은 스크린에 이름이 만들어져버림
	FString Url = BaseURL+ServerIP+ServerResponsePort+ServerCreatePath;
	HttpCall(Url, "GET");
}

void UJMSStartUI::OnFindSession()
{
	if (WidgetSwitcher)
		WidgetSwitcher->SetActiveWidgetIndex(1);
	FString Url = BaseURL+ServerIP+ServerResponsePort+ServerListPath;
	HttpCall(Url, "GET");
}



void UJMSStartUI::OnJoinSession()
{
	
}

void UJMSStartUI::OnDestroySession()
{
	FString DestroyPort = FString("/7777");
	FString Url = BaseURL+ServerIP+ServerResponsePort+ServerDestroyPath+DestroyPort;
	
	HttpCall(Url, "GET");
}



void UJMSStartUI::OnExitButton()
{
	ServerLog(TEXT("OnExitButton"));
}

void UJMSStartUI::OnBackButton()
{
	if (WidgetSwitcher)
		WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UJMSStartUI::ServerLog_Implementation(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
}
