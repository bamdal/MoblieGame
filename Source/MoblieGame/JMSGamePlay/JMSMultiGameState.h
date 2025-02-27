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

	// 플레이어 수 업데이트 함수 (서버에서만 실행)
	void UpdatePlayerCount();

protected:
	// 플레이어 수 변경 시 클라이언트에서 실행될 함수
	UFUNCTION()
	void OnRep_CurrentPlayerCount();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
