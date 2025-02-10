// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"

AEOSPlayerController::AEOSPlayerController()
{
}

void AEOSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Login();
}

void AEOSPlayerController::Login()
{
	// OSS 객체를 통해 인터페이스를 넣는과정 각각 서비스에 따라 다른 인터페이스 적용
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();

	// 로그인 함수가 호출 완료시 콜백함수로 델리게이트가 실행되게 바인딩 되었다.
	const auto delegate = FOnLoginCompleteDelegate::CreateUObject(this, &AEOSPlayerController::HandleLoginCompleted);
	// Player번호를 0으로 넣어둠
	this->LoginDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0,delegate);

	// OSS인터페이스로 로그인을 호출했을때 실패시 델리게이트 삭제후 재설정하는 함수 Player번호를 0으로 넣어둠
	FOnlineAccountCredentials Credentials;
	Credentials.Type = TEXT("developer");  // EOS 로그인 타입 지정
	Credentials.Id = TEXT("127.0.0.1:8081");  // 공백 유지
	Credentials.Token = TEXT("JMSTestUser");  // 공백 유지
	
	if (!Identity->Login(0, Credentials))
	{
		Identity->ClearOnLoginCompleteDelegate_Handle(0,LoginDelegateHandle);
		LoginDelegateHandle.Reset();
	}


}

void AEOSPlayerController::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorMessage)
{
	// OSS함수는 로그인 되기전에 어떠한 EOS의 기능을 사용해선 안된다.
	// OSS 객체를 통해 인터페이스를 넣는과정
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Login CallBack successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("EOS Login Failed"));
	}

	// 실패시 델리게이트 삭제후 재설정하는 함수
	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum,LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}
