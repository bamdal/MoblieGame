// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayGameMode.generated.h"

class AJMSCharBase;
class UJMSMultiGameInstance;
class AJMSDummyButton;
/**
 * 
 */
UCLASS()
class MOBLIEGAME_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()
public:

	APlayGameMode();


	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "JMSDummy")
	TArray<TSubclassOf<AJMSCharBase>> PlayerCharacters;

private:
	UPROPERTY()
	int32 MaxGamePlayerCount;

public:
	[[nodiscard]] int32 GetMaxGamePlayerCount() const;
	void SetMaxGamePlayerCount(int32 MaxGamePlayerCount);

protected:


	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	virtual void Logout(AController* Exiting) override;


	virtual void PostSeamlessTravel() override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	
	void AllowCharacterSelection(APlayerController* NewPlayer);

public:
	UFUNCTION()
	int32 GetCurrentPlayerCount();
	
};
