// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MoblieGame/JMSDummy/JMSDummyButton.h"
#include "JMSGamePlayController.generated.h"


class UJMSMultiGameInstance;
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


	// 서버 RPC (서버에 버튼 색상 변경하라 알림)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestButtonActive(EDummyState DummyCharacterState);

	// 추적자였다가 Runner전환시 버튼 롤백용 함수
	UFUNCTION(Server, Reliable)
	void Server_RequestChaserButtonReset();
public:
	UPROPERTY(ReplicatedUsing=OnRep_DummyButtons)
	TArray<TObjectPtr<AJMSDummyButton>> Buttons;

	UFUNCTION()
	void OnRep_DummyButtons();

	UFUNCTION()
	void ServerLogOut();

	// 해당 맵에 사용할 UI
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget> LobbyHUD;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget> BattleHUD;
	

	//소환된 UI
	UPROPERTY()
	TArray<UUserWidget*> ActiveWidgetArray;

	// UI생성
	UFUNCTION()
	void SetHUD(TSubclassOf<UUserWidget> Widget);

	// UI TEXT 수정
	UFUNCTION()
	void SetCountPlayerUI(int32 CurrentUser);
	UFUNCTION()
	void SetChaserStatusUI(int32 TotalChaserStatus);
	UFUNCTION()
	void SetRunnerCountUI(int32 RunnerCount);
	UFUNCTION()
	void SetCanStartUI(bool CanStart);
	
	//UI 삭제
	UFUNCTION()
	void ClearUI();
private:
	UPROPERTY()
	TSubclassOf<UUserWidget> NewHUD;

	UPROPERTY()
	UJMSMultiGameInstance* GI;
protected:

	
	
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
