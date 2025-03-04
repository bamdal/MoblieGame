// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSChaser.h"

#include "EnhancedInputComponent.h"
#include "JMSChaserAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MoblieGame/JMSGamePlay/JMSGamePlayController.h"
#include "MoblieGame/JMSGamePlay/JMSMultiGameState.h"
#include "Net/UnrealNetwork.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"


AJMSChaser::AJMSChaser()
{
	bReplicates = true;
}


void AJMSChaser::BeginPlay()
{
	Super::BeginPlay();

	ChaserAnimInstance = Cast<UJMSChaserAnimInstance>(GetMesh()->GetAnimInstance());

	if (GetWorld()->GetMapName().Contains("Battle"))
	{
		HideSelfFromCamera();
	}
}


void AJMSChaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled()) // 로컬 플레이어만 실행
	{
		FVector ForwardVector = GetFollowCamera()->GetForwardVector();
		FVector LookAtLocation = GetActorLocation() + ForwardVector * 1000.0f;

		if (HasAuthority()) // 서버라면 직접 설정
		{
			LookAtTarget = LookAtLocation;
		}
		else // 클라이언트라면 서버에 요청
		{
			ServerSetLookAtTarget(LookAtLocation);
		}

		// 1. 카메라 방향 가져오기
		FRotator CameraRotation = GetControlRotation();
		FRotator CameraYaw = CameraRotation;

		// 2. 현재 캐릭터의 기준 방향 (몸의 Yaw)
		float CharacterYaw = GetActorRotation().Yaw;

		// 3. 머리가 몸 기준으로 얼마나 회전했는지 계산
		float HeadYawDelta = FMath::FindDeltaAngleDegrees(CharacterYaw, CameraYaw.Yaw);
		bool bNewShouldTurnBody = FMath::Abs(HeadYawDelta) > 30.0f;

		if (HasAuthority())
		{
			HeadYaw = HeadYawDelta;
			bShouldTurnBody = bNewShouldTurnBody;
		}
		else
		{
			ServerUpdateHeadYaw(HeadYawDelta, bNewShouldTurnBody);
		}


		// 4. 몸 회전 적용 (머리가 60도 이상 돌았을 때)
		if (bNewShouldTurnBody)
		{
			FRotator TargetYaw = CameraYaw;
			if (HasAuthority())
			{
				TargetYawRotation = TargetYaw; // 서버에서 목표 회전값 저장
			}
			else
			{
				ServerSetTargetYaw(TargetYaw); // 클라이언트는 서버에 목표 회전값 요청
			}
		}
	}

	// 5. 모든 클라이언트는 목표 회전값을 받아 부드럽게 회전
	FRotator NewRotation = GetActorRotation();
	float DeltaYaw = FMath::FindDeltaAngleDegrees(NewRotation.Yaw, TargetYawRotation.Yaw);
	NewRotation.Yaw = FMath::FInterpTo(NewRotation.Yaw, NewRotation.Yaw + DeltaYaw, DeltaTime, BodyTurnSpeed);
	SetActorRotation(NewRotation);
}


void AJMSChaser::HideSelfFromCamera()
{
	if (IsLocallyControlled())
	{
		APawn* ControlledPawn = GetController()->GetPawn();
		if (ControlledPawn)
		{
			// ControlledPawn이 자기 자신일 경우 렌더링 비활성화
			ControlledPawn->SetActorHiddenInGame(true);
			SetCameraBoomLength(0.0f);
		}
	}
}

void AJMSChaser::OnRep_LookAtTarget()
{
}

void AJMSChaser::ServerSetLookAtTarget_Implementation(FVector NewLookAtTarget)
{
	LookAtTarget = NewLookAtTarget;
}

bool AJMSChaser::ServerSetLookAtTarget_Validate(FVector NewLookAtTarget)
{
	return true;
}


void AJMSChaser::OnRep_HeadYaw()
{
}

void AJMSChaser::OnRep_ShouldTurnBody()
{
}

void AJMSChaser::OnRep_TargetYawRotation()
{
	// 클라이언트에서 목표 회전값 업데이트 시 호출됨
}

void AJMSChaser::ChaserAttack(const FInputActionValue& InputActionValue)
{
	if (!AttackMontage && !ChaserAnimInstance)
	{
		return;
	}

	if (!ChaserAnimInstance->Montage_IsPlaying(AttackMontage))
	{
		Server_AttackMontage(AttackMontage);
	}
}

void AJMSChaser::Server_AttackMontage_Implementation(UAnimMontage* AnimMontage)
{
	if (HasAuthority())
	{
		Multicast_AttackMontage(AnimMontage);
	}
}

void AJMSChaser::Multicast_AttackMontage_Implementation(UAnimMontage* AnimMontage)
{
	if (!HasAuthority())
	{
		ChaserAnimInstance->Montage_Play(AnimMontage);
	}
}

void AJMSChaser::ChaserCrouch(const FInputActionValue& InputActionValue)
{
	if (bIsCrouched)
	{
		UnCrouch();
		SetCameraBoomRelativeLocation(FVector(8.0f, 10.0f, 168.0f));
	}
	else
	{
		Crouch();
		SetCameraBoomRelativeLocation(FVector(8.0f, 10.0f, 118.0f));
	}
}

void AJMSChaser::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &ThisClass::ChaserAttack);
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ThisClass::ChaserCrouch);
	}
}

void AJMSChaser::PlayStart()
{
	if (AJMSMultiGameState* GS = Cast<AJMSMultiGameState>(GetWorld()->GetGameState()))
	{
		if (GS->CanPlay)
		{
			UE_LOG(LogTemp, Display, TEXT("Play Start"));
			// 술래가 게임 시작 요청을 함 서버에서 진행하게 해야함
			AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(GetController());
			if (PC)
			{
				UE_LOG(LogTemp, Display, TEXT("Try Travel"));
				PC->Server_ServerTravel(FString("/Game/Map/Battle?game=PlayGameMode"), true);

			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Do Not Play Start"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Found GameState"));
	}
}


void AJMSChaser::ServerSetTargetYaw_Implementation(FRotator NewRotation)
{
	TargetYawRotation = NewRotation;
}

bool AJMSChaser::ServerSetTargetYaw_Validate(FRotator NewRotation)
{
	return true;
}


void AJMSChaser::ServerUpdateHeadYaw_Implementation(float NewHeadYaw, bool bNewShouldTurnBody)
{
	HeadYaw = NewHeadYaw;
	bShouldTurnBody = bNewShouldTurnBody;
}

bool AJMSChaser::ServerUpdateHeadYaw_Validate(float NewHeadYaw, bool bNewShouldTurnBody)
{
	return true;
}


void AJMSChaser::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// LookAtTarget 리플리케이션 적용
	DOREPLIFETIME(AJMSChaser, LookAtTarget);
	DOREPLIFETIME(AJMSChaser, HeadYaw);
	DOREPLIFETIME(AJMSChaser, bShouldTurnBody);
	DOREPLIFETIME(AJMSChaser, TargetYawRotation);
}
