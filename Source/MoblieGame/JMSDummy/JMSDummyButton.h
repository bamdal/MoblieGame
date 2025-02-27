// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoblieGame/ETC/JMSEnum.h"
#include "JMSDummyButton.generated.h"

enum class EDummyState : uint8;
class AJMSGamePlayController;
enum class EButtonState : uint8;

UCLASS()
class MOBLIEGAME_API AJMSDummyButton : public AActor
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJMSDummyButton();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Button")
	USceneComponent* Root;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Button")
	UStaticMeshComponent* ButtonBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Button")
	UStaticMeshComponent* ButtonCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Button")
	TObjectPtr<UMaterialInterface> UnSelectedButtonMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Button")
	TObjectPtr<UMaterialInterface> SelectedButtonMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Button")
	TObjectPtr<UMaterialInterface> DisableButtonMaterial;


private:
	// 버튼 상태 관리
	UPROPERTY(ReplicatedUsing = OnRep_DummyButtonState)
	EButtonState DummyButtonState;

	UPROPERTY()
	AJMSGamePlayController* NewPlayer;
	
	// 버튼의 부모가 가지고 있던 상태
	UPROPERTY()
	EDummyState DummyCharacterState = EDummyState::Chaser;
	

public:
	[[nodiscard]] EButtonState GetDummyButtonState() const
	{
		return DummyButtonState;
	}

	[[nodiscard]] EDummyState GetDummyCharacterState() const;

	void SetDummyButtonState(EButtonState NewDummyButtonState)
	{
		this->DummyButtonState = NewDummyButtonState;
	}

	UFUNCTION()
	void OnRep_DummyButtonState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	// 버튼 상태 변경
	UFUNCTION()
	void UpdateButtonState(EButtonState NewState, AJMSGamePlayController* Player=nullptr);

	UFUNCTION()
	void ResetButtonState();

	UFUNCTION()
	void ResetChaserButtonState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};

