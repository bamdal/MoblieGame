// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSRunner.h"

#include "GameFramework/SpringArmComponent.h"


AJMSRunner::AJMSRunner()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));

	GetCameraBoom()->TargetArmLength = 200.0f;
}

void AJMSRunner::BeginPlay()
{
	Super::BeginPlay();
    
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("AJMSRunner::BeginPlay - MovementComponent is valid"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AJMSRunner::BeginPlay - MovementComponent is MISSING!"));
	}
}

void AJMSRunner::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AJMSRunner::SetupPlayerInputComponent - PlayerInputComponent is NULL"));
	}
}
