// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "JMSRunner.generated.h"

/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AJMSRunner : public AJMSCharBase
{
	GENERATED_BODY()
public:
	AJMSRunner();

	virtual void BeginPlay() override;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
};
