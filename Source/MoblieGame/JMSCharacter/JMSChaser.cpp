// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSChaser.h"

#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"


AJMSChaser::AJMSChaser()
{
}


void AJMSChaser::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	//HideSelfFromCamera();
}

void AJMSChaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled()) // 로컬 플레이어만 실행
	{
		FVector ForwardVector = GetFollowCamera()->GetForwardVector();
		FVector LookAtLocation = GetActorLocation() + ForwardVector * 100.0f;

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
		float CameraYaw = CameraRotation.Yaw;

		// 2. 현재 캐릭터의 기준 방향 (몸의 Yaw)
		float CharacterYaw = GetActorRotation().Yaw;

		// 3. 머리가 몸 기준으로 얼마나 회전했는지 계산
		float HeadYawDelta = FMath::FindDeltaAngleDegrees(CharacterYaw, CameraYaw);
		bool bNewShouldTurnBody = FMath::Abs(HeadYawDelta) > 60.0f;

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
			float TargetYaw = CameraYaw; // 카메라 방향으로 몸을 돌림
			FRotator NewRotation = GetActorRotation();

			// 🔹 왼쪽(-) 방향 회전일 경우 보정
			if (HeadYawDelta < -60.0f)
			{
				TargetYaw = CharacterYaw - (FMath::Abs(HeadYawDelta) - 60.0f);
			}
			else if (HeadYawDelta > 60.0f)
			{
				TargetYaw = CharacterYaw + (FMath::Abs(HeadYawDelta) - 60.0f);
			}

			NewRotation.Yaw = FMath::FInterpTo(NewRotation.Yaw, TargetYaw, DeltaTime, BodyTurnSpeed);
			if (HasAuthority())
			{
				SetActorRotation(NewRotation);
				ReplicatedRotation = NewRotation; // 서버에서 회전 값 설정
			}
			else
			{
				ServerSetRotation(NewRotation); // 클라이언트는 서버에 요청
			}
		}
	}
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

void AJMSChaser::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// LookAtTarget 리플리케이션 적용
	DOREPLIFETIME(AJMSChaser, LookAtTarget);
	DOREPLIFETIME(AJMSChaser, HeadYaw);
	DOREPLIFETIME(AJMSChaser, bShouldTurnBody);
	DOREPLIFETIME(AJMSChaser, ReplicatedRotation);
	
}


void AJMSChaser::OnRep_HeadYaw()
{
}

void AJMSChaser::OnRep_ShouldTurnBody()
{
}

void AJMSChaser::ServerSetRotation_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
	ReplicatedRotation = NewRotation;
}

bool AJMSChaser::ServerSetRotation_Validate(FRotator NewRotation)
{
	return true;
}

void AJMSChaser::OnRep_UpdateRotation()
{
	SetActorRotation(ReplicatedRotation);
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