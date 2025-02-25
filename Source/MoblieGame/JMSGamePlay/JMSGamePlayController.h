// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JMSGamePlayController.generated.h"

class AJMSCharBase;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AJMSGamePlayController : public APlayerController
{
	GENERATED_BODY()

public:
	
	// 서버 RPC (서버에 캐릭터 스폰하라 알림)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestResponseCharacter(TSubclassOf<AJMSCharBase> Char,FVector Location);

	// 모든 클라이언트에게 상태 동기화
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateResponseCharacter(TSubclassOf<AJMSCharBase> Char,FVector Location);
	
	
};
