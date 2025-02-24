// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JMSChaserAnimInstance.generated.h"

class AJMSChaser;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API UJMSChaserAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadWrite)
	FVector LookAtTarget; // LookAt 좌표

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	float HeadYaw; // 머리 회전 각도

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	bool bShouldTurnBody; // 몸이 회전해야 하는지 여부

protected:

	UPROPERTY()
	AJMSChaser* OwnerCharacter;
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;


	
	
};
