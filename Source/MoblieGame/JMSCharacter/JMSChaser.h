// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "JMSChaser.generated.h"

/**
 * 
 */
UCLASS()
class MOBLIEGAME_API AJMSChaser : public AJMSCharBase
{
	GENERATED_BODY()

	AJMSChaser();

	UFUNCTION()
	void HideSelfFromCamera();

public:
	// 머리 돌리기
	// LookAt 벡터 (리플리케이션)
	UPROPERTY(ReplicatedUsing = OnRep_LookAtTarget)
	FVector LookAtTarget;

	UFUNCTION()
	void OnRep_LookAtTarget(); // 클라이언트에서 값이 변경될 때 호출

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetLookAtTarget(FVector NewLookAtTarget);

	// 몸통 돌리기
	UPROPERTY(ReplicatedUsing = OnRep_HeadYaw)
	float HeadYaw;

	UPROPERTY(ReplicatedUsing = OnRep_ShouldTurnBody)
	bool bShouldTurnBody;

	UPROPERTY(ReplicatedUsing=OnRep_UpdateRotation)
	FRotator ReplicatedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Rotation")
	float BodyTurnSpeed = 5.0f;

	UFUNCTION()
	void OnRep_HeadYaw();

	UFUNCTION()
	void OnRep_ShouldTurnBody();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateHeadYaw(float NewHeadYaw, bool bNewShouldTurnBody);

	private:
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetRotation(FRotator NewRotation);

    UFUNCTION()
    void OnRep_UpdateRotation();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
