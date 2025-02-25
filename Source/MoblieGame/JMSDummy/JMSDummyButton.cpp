// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSDummyButton.h"

#include "MoblieGame/ETC/JMSEnum.h"
#include "MoblieGame/JMSCharacter/JMSCharBase.h"
#include "MoblieGame/JMSGamePlay/JMSGamePlayController.h"
#include "MoblieGame/JMSInterface/ButtonClickInterface.h"
#include "Net/UnrealNetwork.h"

AJMSDummyButton* AJMSDummyButton::CurrentChaserButton = nullptr; // 현재 술래 버튼 초기화

// Sets default values
AJMSDummyButton::AJMSDummyButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBase"));
	ButtonBase->SetupAttachment(Root);

	ButtonBase->SetRelativeScale3D(FVector(3.25f, 3.25f, 0.5f));

	ButtonCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonCenter"));
	ButtonCenter->SetupAttachment(ButtonBase);
	ButtonCenter->SetRelativeLocation(FVector(-37.0f, -37.0f, 20.0f));
	ButtonCenter->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.0f));

	ButtonState = EButtonState::Normal;
}

// Called when the game starts or when spawned
void AJMSDummyButton::BeginPlay()
{
	Super::BeginPlay();
	ButtonCenter->OnComponentHit.AddDynamic(this, &AJMSDummyButton::OnComponentHit);
}

// Called every frame
void AJMSDummyButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AJMSDummyButton::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                     const FHitResult& Hit)
{
	if (ButtonState != EButtonState::Normal)
	{
		return;
	}

	// 플레이어가 닿으면 해당 캐릭터로 변환
	UE_LOG(LogTemp, Warning, TEXT("버튼 닿음"));

	AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(OtherActor->GetInstigatorController());
	if (PC && PC->IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("PC처리"));

		UpdateButtonState(EButtonState::Selected,PC);
	}
}

void AJMSDummyButton::UpdateButtonState(EButtonState NewState, AJMSGamePlayController* Player)
{
	ButtonState = NewState;

	switch (ButtonState)
	{
	case EButtonState::Normal:
		ButtonCenter->SetMaterial(0, UnSelectedButtonMaterial);
		break;
	case EButtonState::Selected:
		ButtonCenter->SetMaterial(0, SelectedButtonMaterial);
		if (GetParentActor() && GetParentActor()->GetClass()->ImplementsInterface(UButtonClickInterface::StaticClass()))
		{
			AJMSCharBase* ResultChar = Cast<AJMSCharBase>(Player->GetCharacter());
			if (IButtonClickInterface::Execute_DummyButtonClick(GetParentActor(),ResultChar))
			{
				// 서버가 받아야함
				UpdateButtonState(EButtonState::Disabled);
			}
			if (ResultChar)
			{
				UE_LOG(LogTemp,Warning,TEXT("ResultChar : %s"),*ResultChar->GetClass()->GetName());
			}
		}
		break;
	case EButtonState::Disabled:
		ButtonCenter->SetMaterial(0, DisableButtonMaterial);
		break;
	}
}
