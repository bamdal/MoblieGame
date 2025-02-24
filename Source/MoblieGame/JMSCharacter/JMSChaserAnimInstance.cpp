// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSChaserAnimInstance.h"

#include "JMSChaser.h"


void UJMSChaserAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AJMSChaser>(TryGetPawnOwner());
}

void UJMSChaserAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		LookAtTarget = OwnerCharacter->LookAtTarget;
		bShouldTurnBody = OwnerCharacter->bShouldTurnBody;
		HeadYaw = OwnerCharacter->HeadYaw;
	}
}

