// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "JMSMultiGameState.generated.h"

class APlayGameMode;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AJMSMultiGameState : public AGameState
{
	GENERATED_BODY()
	
	AJMSMultiGameState();

	// 현재 게임모드가 참조하는 GameMode
	UPROPERTY()
	APlayGameMode* AuthGM;
	
public:

	// 현재 플레이어 수
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerCount, BlueprintReadOnly)
	int32 CurrentPlayerCount = 0;

	// 술래 유무 (0 ~ 1)
	UPROPERTY(ReplicatedUsing = OnRep_CurrentChaserCount, BlueprintReadOnly)
	int32 CurrentChaserCount = 0;

	// 도망자 수 (0 ~ n-1)
	UPROPERTY(ReplicatedUsing = OnRep_CurrentRunnerCount, BlueprintReadOnly)
	int32 CurrentRunnerCount = 0;

	// 시작 여부 = 현재 플레이어 n = 술래가능 1 + 도망자 n-1 && 술래가능 1
	UPROPERTY(ReplicatedUsing = OnRep_CanPlay, BlueprintReadOnly)
	bool CanPlay = false;
	/*

	현재 플레이어 수의 업데이트가 생기면 술래, 도망자 정보를 업데이트 해야한다.
	OnRep_CurrentPlayerCount에서 업데이트

	*/

	// 플레이어 수 업데이트 함수 (서버에서만 실행)
	void UpdatePlayerCount();



public:
	// 플레이어 수 변경 시 클라이언트에서 실행될 함수
	UFUNCTION()
	void OnRep_CurrentPlayerCount();

	UFUNCTION()
	void OnRep_CurrentChaserCount();
	UFUNCTION()
	void OnRep_CurrentRunnerCount();
	UFUNCTION()
	void OnRep_CanPlay();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
