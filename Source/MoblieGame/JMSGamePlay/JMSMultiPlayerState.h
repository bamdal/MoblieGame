// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MoblieGame/ETC/JMSEnum.h"
#include "JMSMultiPlayerState.generated.h"

enum class EDummyState : uint8;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AJMSMultiPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	AJMSMultiPlayerState();
public:
	// 각 플레이어들의 현재 어떤 역할을 가지고 있는지 공유한다.
	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite)
	EDummyState PlayerCharacterRoleState = EDummyState::Runner_None;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};
