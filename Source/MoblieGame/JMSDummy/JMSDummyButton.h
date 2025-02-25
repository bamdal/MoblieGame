// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JMSDummyButton.generated.h"

class AJMSGamePlayController;
enum class EButtonState : uint8;

UCLASS()
class MOBLIEGAME_API AJMSDummyButton : public AActor
{
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


	// 버튼 상태 관리
	UPROPERTY()
	EButtonState ButtonState;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 버튼 상태 변경
	UFUNCTION()
	void UpdateButtonState(EButtonState NewState, AJMSGamePlayController* Player=nullptr);


private:
	static AJMSDummyButton* CurrentChaserButton;  // 현재 술래 버튼 (전역적으로 한 개만 존재)
};

