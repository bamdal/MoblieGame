// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "JMSChaser.generated.h"

class UJMSChaserAnimInstance;
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


	UPROPERTY(ReplicatedUsing = OnRep_TargetYawRotation)
	FRotator TargetYawRotation;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Rotation")
	float BodyTurnSpeed = 5.0f;

	UPROPERTY()
	float LastRotationUpdateTime;

	UFUNCTION()
	void OnRep_HeadYaw();

	UFUNCTION()
	void OnRep_ShouldTurnBody();

	UFUNCTION()
	void OnRep_TargetYawRotation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateHeadYaw(float NewHeadYaw, bool bNewShouldTurnBody);

	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Crouch;

	// 서버에 공격하라 알림
	UFUNCTION()
	void ChaserAttack(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Input")
	TObjectPtr<UAnimMontage> AttackMontage;

	FOnMontageStarted OnAttackMontageStart;
	FOnMontageEnded OnAttackMontageEnded;

	UFUNCTION(Server, Reliable)
	void Server_AttackMontage(UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttackMontage(UAnimMontage* AnimMontage);

	UPROPERTY()
	UJMSChaserAnimInstance* ChaserAnimInstance;

	// 앉기 수행
	UFUNCTION()
	void ChaserCrouch(const FInputActionValue& InputActionValue);
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PlayStart() override;
private:
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetTargetYaw(FRotator NewRotation);


	

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
