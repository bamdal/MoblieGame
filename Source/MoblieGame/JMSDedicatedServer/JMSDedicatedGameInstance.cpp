// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSDedicatedGameInstance.h"


void UJMSDedicatedGameInstance::TravelSession()
{
	// 게임 시작 시 서버 접속
	FString ServerIP = TEXT("35.188.109.159:7777"); // 접속할 서버의 IP 주소
	FString Command = FString::Printf(TEXT("open %s"), *ServerIP);
	GetFirstLocalPlayerController(GetWorld())->ConsoleCommand(Command);
	UE_LOG(LogTemp, Display, TEXT("%s"), *Command);
}

void UJMSDedicatedGameInstance::LocalTravelSession()
{
	FString ServerIP = TEXT("192.168.0.159:7777"); // 접속할 서버의 IP 주소
	FString Command = FString::Printf(TEXT("open %s"), *ServerIP);
	GetFirstLocalPlayerController(GetWorld())->ConsoleCommand(Command);
	UE_LOG(LogTemp, Display, TEXT("%s"), *Command);
}
