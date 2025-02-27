// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSDummyButton.h"

#include "Kismet/GameplayStatics.h"
#include "MoblieGame/ETC/JMSEnum.h"
#include "MoblieGame/JMSCharacter/JMSCharBase.h"
#include "MoblieGame/JMSGamePlay/JMSGamePlayController.h"
#include "MoblieGame/JMSInterface/ButtonClickInterface.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AJMSDummyButton::AJMSDummyButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBase"));
	ButtonBase->SetupAttachment(Root);

	ButtonBase->SetRelativeScale3D(FVector(3.25f, 3.25f, 0.5f));

	ButtonCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonCenter"));
	ButtonCenter->SetupAttachment(ButtonBase);
	ButtonCenter->SetRelativeLocation(FVector(-37.0f, -37.0f, 20.0f));
	ButtonCenter->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.0f));

	DummyButtonState = EButtonState::Normal;
}

DummyState AJMSDummyButton::GetDummyCharacterState() const
{
	return DummyCharacterState;
}

void AJMSDummyButton::OnRep_DummyButtonState()
{
	switch (DummyButtonState)
	{
	case EButtonState::Normal:
		ButtonCenter->SetMaterial(0, UnSelectedButtonMaterial);
		break;
	case EButtonState::Selected:
		ButtonCenter->SetMaterial(0, SelectedButtonMaterial);
		if (GetParentActor() && GetParentActor()->GetClass()->ImplementsInterface(UButtonClickInterface::StaticClass()))
		{
			if (NewPlayer)
			{
				AJMSCharBase* ResultChar = Cast<AJMSCharBase>(NewPlayer->GetCharacter());
				// 캐릭터 변환
				UE_LOG(LogTemp, Warning, TEXT("캐릭터 바꾸기 버튼"));

				if (IButtonClickInterface::Execute_DummyButtonClick(GetParentActor(), ResultChar))
				{
					// 각 플레이어에게 술래발판이 활성화 됨을 알림
					NewPlayer->Server_RequestButtonActive(GetDummyCharacterState());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Runner 바꾸기"));
					// 각 플레이어에게 일반발판이 활성화 됨을 알림
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(
						TimerHandle,
						this,
						&AJMSDummyButton::ResetButtonState,
						0.5f,
						false
					);
				}
				if (ResultChar)
				{
					UE_LOG(LogTemp, Warning, TEXT("OldChar : %s"), *ResultChar->GetClass()->GetName());
				}
			}
		}
		break;
	case EButtonState::Disabled:
		ButtonCenter->SetMaterial(0, DisableButtonMaterial);
		UE_LOG(LogTemp, Warning, TEXT("EButtonState::Disabled"));
		break;
	}
}

// Called when the game starts or when spawned
void AJMSDummyButton::BeginPlay()
{
	Super::BeginPlay();
	ButtonCenter->OnComponentHit.AddDynamic(this, &AJMSDummyButton::OnComponentHit);

	if (GetParentActor() && GetParentActor()->GetClass()->ImplementsInterface(UButtonClickInterface::StaticClass()))
	{
		DummyCharacterState = IButtonClickInterface::Execute_GetDummyCharacterState(GetParentActor());
		UE_LOG(LogTemp, Warning, TEXT("Button State : %hhd"), DummyCharacterState);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Button State : None"));
	}
}

// Called every frame
void AJMSDummyButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AJMSDummyButton::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                     const FHitResult& Hit)
{
	if (DummyButtonState != EButtonState::Normal)
	{
		return;
	}

	if (!HasAuthority())
		return;

	// 플레이어가 닿으면 해당 캐릭터로 변환
	UE_LOG(LogTemp, Warning, TEXT("버튼 닿음"));

		AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(OtherActor->GetInstigatorController());
		if (PC && DummyCharacterState != PC->PlayerGameRoleState)
		{

			UE_LOG(LogTemp, Warning, TEXT("PC처리"));

			if (PC->PlayerGameRoleState==DummyState::Chaser)
			{
				PC->Server_RequestChaserButtonReset();
			}

			UpdateButtonState(EButtonState::Selected, PC);
			PC->PlayerGameRoleState = DummyCharacterState;
		}
	
}

void AJMSDummyButton::UpdateButtonState(EButtonState NewState, AJMSGamePlayController* Player)
{
	if (HasAuthority()) // 서버에서만 직접 변경
	{
		DummyButtonState = NewState;
		NewPlayer = Player;
		OnRep_DummyButtonState();
	}
}

void AJMSDummyButton::ResetButtonState()
{
	UpdateButtonState(EButtonState::Normal, nullptr);
}

void AJMSDummyButton::ResetChaserButtonState()
{
	if (DummyCharacterState == DummyState::Chaser)
	{
		ResetButtonState();
	}
}


void AJMSDummyButton::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJMSDummyButton, DummyButtonState);
}
