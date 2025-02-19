// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSStartUI.h"

#include "JMSButton.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Components/Button.h"


void UJMSStartUI::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnCreateSession);
	JoinSessionButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnJoinSession);
	ExitButton->JMSButton->OnClicked.AddDynamic(this,&ThisClass::OnExitButton);

	UE_LOG(LogTemp, Display, TEXT("ServerUsername: %s"), *ServerUsername);
	UE_LOG(LogTemp, Display, TEXT("ServerIP: %s"), *ServerIP);
	UE_LOG(LogTemp, Display, TEXT("ServerScriptPath: %s"), *ServerScriptPath);
	UE_LOG(LogTemp, Display, TEXT("ServerScriptName: %s"), *ServerScriptName);
	UE_LOG(LogTemp, Display, TEXT("BasePort: %d"), BasePort);

}
void UJMSStartUI::OnCreateSession()
{
	/*UE_LOG(LogTemp, Display, TEXT("OnCreateSession"));

	// 🔥 사용 가능한 포트 찾기
	FString FindPortCommand = FString::Printf(
		TEXT("ssh %s@%s \"screen -ls | grep server_ | awk -F'_' '{print $2}' | sort -n\""),
		*ServerUsername, *ServerIP
	);
	// cmd.exe를 사용하여 SSH 명령 실행
	FString CmdLine = FString::Printf(TEXT("/C \"%s\""), *FindPortCommand);  // /C는 cmd 명령어가 끝난 후 종료되게 함

	FString UsedPorts;
	FString StdErr;
	int32 ReturnCode;

#if PLATFORM_WINDOWS
	// PC 전용 모바일용도 만들어야함
	FPlatformProcess::ExecProcess(TEXT("cmd.exe"), *CmdLine, &ReturnCode, &UsedPorts, &StdErr);

	if (ReturnCode != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get used ports. Error: %s"), *StdErr);
		return;
	}
#endif

#if PLATFORM_ANDROID
	// Android에서는 Runtime을 사용하여 명령어 실행
	const FString Command = FString::Printf(TEXT("sh -c \"%s\""), *FindPortCommand);

	// ExecProcess 사용을 피하고, Android에서 명령어 실행

	jobject activity = AndroidApplication::GetGameActivity();
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jmethodID Method = Env->GetMethodID(FAndroidApplication::FindJavaClass("com/epicgames/ue4/GameActivity"), "runShellCommand", "(Ljava/lang/String;)Ljava/lang/String;");
	jstring jCommand = Env->NewStringUTF(*Command);

	jstring result = (jstring)Env->CallObjectMethod(activity, Method, jCommand);

	const char* nativeResult = Env->GetStringUTFChars(result, 0);
	UsedPorts = FString(nativeResult);
	StdErr = TEXT(""); // 에러 처리는 필요한 경우 추가

	Env->ReleaseStringUTFChars(result, nativeResult);
#endif
	


	// 🔥 사용 중인 포트 리스트 파싱
	TArray<int32> UsedPortList;
	TArray<FString> PortStrings;
	UsedPorts.ParseIntoArrayLines(PortStrings);

	for (const FString& PortString : PortStrings)
	{
		int32 Port = FCString::Atoi(*PortString);
		UsedPortList.Add(Port);
	}

	// 🔥 다음 사용 가능한 포트 찾기
	int32 NewPort = BasePort;

	for (int i = 0; i < MaxServers; i++)
	{
		NewPort = BasePort + i;
		if (!UsedPortList.Contains(NewPort))
		{
			break;
		}
	}

	// 🔥 실행할 맵 이름을 설정 (기본값: ThirdPersonMap)
	FString SelectedMap = "ThirdPersonMap";  // 기본 맵
	// 필요하면 UI에서 맵 선택 기능 추가 가능

	// 🔥 SSH 명령어 생성 및 실행 (맵 이름도 전달)
	FString StartServerCommand = FString::Printf(
		TEXT("ssh %s@%s 'cd %s && screen -dmS server_%d ./%s %d %s'"),
		*ServerUsername, *ServerIP, *ServerScriptPath, NewPort, *ServerScriptName, NewPort, *SelectedMap
	);

	FString StdOut;
	FPlatformProcess::ExecProcess(TEXT("/bin/sh"), *StartServerCommand, &ReturnCode, &StdOut, &StdErr);

	if (ReturnCode == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Server started successfully on port %d with map %s"), NewPort, *SelectedMap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to start server on port %d. Error: %s"), NewPort, *StdErr);
	}*/
}


void UJMSStartUI::OnJoinSession()
{
	/*UE_LOG(LogTemp, Display, TEXT("OnJoinSession"));

	// 서버 리스트 (기본 포트에서 10개까지 검색)
	TArray<int> ServerPorts;
	for (int i = 0; i < 10; i++)
	{
		ServerPorts.Add(BasePort + i);
	}

	for (int Port : ServerPorts)
	{
		FString ServerAddress = FString::Printf(TEXT("%s:%d"), *ServerIP, Port);

		UE_LOG(LogTemp, Display, TEXT("Trying to connect to: %s"), *ServerAddress);

		// 언리얼 네트워크 접속 시도
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(ServerAddress, ETravelType::TRAVEL_Absolute);
			UE_LOG(LogTemp, Display, TEXT("Connected to server on port %d"), Port);
			return;
		}
	}

	// 접속 실패
	UE_LOG(LogTemp, Error, TEXT("Failed to connect to any server."));*/

}


void UJMSStartUI::OnExitButton()
{
	UE_LOG(LogTemp, Display, TEXT("OnExitButton"));
}
