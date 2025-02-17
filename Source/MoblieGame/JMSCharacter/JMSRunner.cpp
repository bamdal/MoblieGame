// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSRunner.h"

#include "GameFramework/SpringArmComponent.h"


AJMSRunner::AJMSRunner()
{
	PrimaryActorTick.bCanEverTick = true;

	
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));

	GetCameraBoom()->TargetArmLength = 200.0f;
}
