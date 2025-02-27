// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoblieGame/JMSInterface/ButtonClickInterface.h"
#include "JMSDummy.generated.h"


class AJMSCharBase;
enum class DummyState : uint8;

UCLASS()
class MOBLIEGAME_API AJMSDummy : public AActor, public IButtonClickInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJMSDummy();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "JMSDummy")
	DummyState DummyState;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "JMSDummy")
	TArray<TSubclassOf<AJMSCharBase>> PlayerCharacters;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 술래면 true
	virtual bool DummyButtonClick_Implementation(AJMSCharBase* Char) override;

	virtual enum DummyState GetDummyCharacterState_Implementation() override;
	

	
};
