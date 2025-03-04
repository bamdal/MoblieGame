// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSGamePlayController.h"

#include "EngineUtils.h"
#include "JMSMultiGameInstance.h"
#include "JMSMultiGameState.h"
#include "JMSMultiPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MoblieGame/JMSCharacter/JMSCharBase.h"
#include "MoblieGame/JMSInterface/LobbyHUDInterface.h"
#include "Net/UnrealNetwork.h"


void AJMSGamePlayController::Server_RequestResponseCharacter_Implementation(TSubclassOf<AJMSCharBase> Char, FVector Location)
{
	Multicast_UpdateResponseCharacter(Char, Location);
}

bool AJMSGamePlayController::Server_RequestResponseCharacter_Validate(TSubclassOf<AJMSCharBase> Char, FVector Location)
{
	return true;
}

void AJMSGamePlayController::Multicast_UpdateResponseCharacter_Implementation(TSubclassOf<AJMSCharBase> Char, FVector Location)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	APawn* OldNonePawn = GetPawn();
	if (OldNonePawn == nullptr)
		return;
	if (HasAuthority())
	{
		FRotator NewRotation = GetPawn()->GetControlRotation();
		UnPossess();
		Possess(GetWorld()->SpawnActor<AJMSCharBase>(Char, Location, NewRotation, SpawnParams));
		OldNonePawn->Destroy();
	}

	if (GetPawn())
	{
		ClientRestart(GetPawn());

		// Possess 후 MovementComponent 체크
		UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetPawn()->GetMovementComponent());
		if (MoveComp)
		{
			MoveComp->SetMovementMode(MOVE_Walking);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("New Pawn has no valid MovementComponent!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to possess new character"));
	}
}


void AJMSGamePlayController::Server_RequestButtonActive_Implementation(EDummyState DummyCharacterState)
{
	if (DummyCharacterState == EDummyState::Chaser)
	{
		for (auto Element : Buttons)
		{
			if (Element->GetDummyCharacterState() == EDummyState::Chaser)
			{
				Element->UpdateButtonState(EButtonState::Disabled);
				break;
			}
		}
	}
}

bool AJMSGamePlayController::Server_RequestButtonActive_Validate(EDummyState DummyCharacterState)
{
	return true;
}

void AJMSGamePlayController::Server_RequestChaserButtonReset_Implementation()
{
	for (auto Element : Buttons)
	{
		UE_LOG(LogTemp, Error, TEXT("Chaser button reset"));
		Element->ResetChaserButtonState();
	}
}


void AJMSGamePlayController::OnRep_DummyButtons()
{
}

void AJMSGamePlayController::ServerLogOut()
{
	AJMSMultiPlayerState* PS = GetPlayerState<AJMSMultiPlayerState>();
	if (PS->GetPlayerCharacterRoleState() == EDummyState::Chaser)
	{
		// 종료될시 술래라면 버튼 롤백
		Server_RequestChaserButtonReset();
	}
	
		
	PS->SetPlayerCharacterRoleState(EDummyState::Runner_None);

}


void AJMSGamePlayController::Server_UpdateChaserCount_Implementation(int32 NewValue)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		AJMSMultiGameState* GS = Cast<AJMSMultiGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CurrentChaserCount = NewValue;
			UE_LOG(LogTemp,Error,TEXT("CurrentChaserCount %d"),GS->CurrentChaserCount);
		}
	}
}

void AJMSGamePlayController::Server_UpdateRunnerCount_Implementation(int32 NewValue)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		AJMSMultiGameState* GS = Cast<AJMSMultiGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CurrentRunnerCount = NewValue;
			UE_LOG(LogTemp,Error,TEXT("CurrentRunnerCount %d"),GS->CurrentRunnerCount);
		}
	}
}

void AJMSGamePlayController::Server_UpdateCanPlay_Implementation(bool NewValue)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		AJMSMultiGameState* GS = Cast<AJMSMultiGameState>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CanPlay = NewValue;
		}
	}
}


void AJMSGamePlayController::SetHUD(TSubclassOf<UUserWidget> Widget)
{


	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
	
	if (NewWidget != nullptr)
	{
		ActiveWidgetArray.Add(NewWidget);
		NewWidget->AddToViewport();
	}
}

void AJMSGamePlayController::SetCountPlayerUI(int32 CurrentUser)
{
	for (UUserWidget* Widget : ActiveWidgetArray)
	{
		if(Widget->GetClass()->ImplementsInterface(ULobbyHUDInterface::StaticClass()))
		{
			ILobbyHUDInterface::Execute_SetTextToTextBlock_TotalPlayers(Widget,CurrentUser);
		}
	}
}

void AJMSGamePlayController::SetChaserStatusUI(int32 TotalChaserStatus)
{
	for (UUserWidget* Widget : ActiveWidgetArray)
	{
		if(Widget->GetClass()->ImplementsInterface(ULobbyHUDInterface::StaticClass()))
		{
			ILobbyHUDInterface::Execute_SetTextToTextBlock_ChaserStatus(Widget,TotalChaserStatus);
		}
	}
}

void AJMSGamePlayController::SetRunnerCountUI(int32 RunnerCount)
{
	for (UUserWidget* Widget : ActiveWidgetArray)
	{
		if(Widget->GetClass()->ImplementsInterface(ULobbyHUDInterface::StaticClass()))
		{
			ILobbyHUDInterface::Execute_SetTextToTextBlock_RunnerCount(Widget,RunnerCount);
		}
	}
}

void AJMSGamePlayController::SetCanStartUI(bool CanStart)
{
	for (UUserWidget* Widget : ActiveWidgetArray)
	{
		if(Widget->GetClass()->ImplementsInterface(ULobbyHUDInterface::StaticClass()))
		{
			ILobbyHUDInterface::Execute_SetTextToTextBlock_CanStart(Widget,CanStart);
		}
	}
}


void AJMSGamePlayController::ClearUI()
{
	for (auto Element : 	ActiveWidgetArray)
	{
		Element->RemoveFromParent();
	}
}

void AJMSGamePlayController::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		for (TActorIterator<AJMSDummyButton> It(GetWorld()); It; ++It)
		{
			Buttons.Add(*It);
		}
	}

	if (GetWorld()->GetMapName().Contains("Lobby"))
	{
		SetHUD(LobbyHUD);
	}
	else if (GetWorld()->GetMapName().Contains("Battle"))
	{
		
		SetHUD(BattleHUD);
	}	
}

void AJMSGamePlayController::Destroyed()
{

	ClearUI();
	
	Super::Destroyed();
	
}


void AJMSGamePlayController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJMSGamePlayController, Buttons);
}
