// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"

#include "EngineUtils.h"
#include "JMSGamePlayController.h"
#include "JMSMultiGameState.h"
#include "JMSMultiPlayerState.h"
#include "GameFramework/PlayerStart.h"

APlayGameMode::APlayGameMode()
{
	bUseSeamlessTravel = true;
}


int32 APlayGameMode::GetMaxGamePlayerCount() const
{
	return MaxGamePlayerCount;
}

void APlayGameMode::SetMaxGamePlayerCount(int32 NewMaxGamePlayerCount)
{
	this->MaxGamePlayerCount = NewMaxGamePlayerCount;
}

void APlayGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void APlayGameMode::StartPlay()
{
	// 처음 시작시 대기상태
	bDelayedStart = true;
	Super::StartPlay();
}

void APlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("현재 MatchState : %s "), *GetMatchState().ToString());

	GetWorld()->GetTimerManager().SetTimerForNextTick([NewPlayer]()
	{
		FInputModeGameOnly InputMode;
		NewPlayer->SetInputMode(InputMode);
		NewPlayer->bShowMouseCursor = false; // 마우스 커서를 숨김
	});
	if (HasMatchStarted())
	{
		// 게임 시작중 진입하면 스펙터로 전환
		NewPlayer->StartSpectatingOnly();
		FHitResult Hit;
		NewPlayer->K2_SetActorLocation(FVector(0,-2400,1500),false,Hit,true);
		UE_LOG(LogTemp, Warning, TEXT("새 플레이어 %s 가 Spectator로 설정됨."), *NewPlayer->GetName());
	}
	else
	{
		// 로비에서 대기 중이라면 캐릭터 선택 가능
		AllowCharacterSelection(NewPlayer);
		UE_LOG(LogTemp, Warning, TEXT("새 플레이어가 캐릭터를 선택할 수 있음."));
	}

	// 전체인원수 갱신
	// GameState 가져오기
	AJMSMultiGameState* GS = GetGameState<AJMSMultiGameState>();
	if (GS)
	{
		GS->UpdatePlayerCount();
	}
}


void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void APlayGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// 전체 인원수 갱신
	// GameState 가져오기
	AJMSMultiGameState* GS = GetGameState<AJMSMultiGameState>();
	if (GS)
	{
		GS->UpdatePlayerCount();
	}
}


void APlayGameMode::PostSeamlessTravel()
{
	UE_LOG(LogTemp,Error,TEXT("PostSeamlessTravelStart"));
	Super::PostSeamlessTravel();
	UE_LOG(LogTemp,Error,TEXT("PostSeamlessTravelEnd"));

	
}

void APlayGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	EDummyState CharRole = EDummyState::Runner_None;
	if (AJMSMultiPlayerState* PS = Cast<AJMSMultiPlayerState>(C->PlayerState))
	{
		UE_LOG(LogTemp, Log, TEXT("HandleSeamlessTravelPlayerStart: PlayerState 유지됨 -> %s"), *PS->GetPlayerName());
		CharRole = PS->GetPlayerCharacterRoleState();
	}

	Super::HandleSeamlessTravelPlayer(C);

	if (AJMSMultiPlayerState* PS = Cast<AJMSMultiPlayerState>(C->PlayerState))
	{
		// ✅ 기존의 PlayerState 데이터를 동기화
		UE_LOG(LogTemp, Log, TEXT("HandleSeamlessTravelPlayerEnd: PlayerState 유지됨 -> %s"), *PS->GetPlayerName());
		PS->SetPlayerCharacterRoleState(CharRole);

		if (PS)
		{
			int32 Index = 0;
			UE_LOG(LogTemp,Error,TEXT("PC %hhd"),PS->GetPlayerCharacterRoleState());

			switch (PS->GetPlayerCharacterRoleState())
			{
			case EDummyState::Chaser:
				Index = 0;
				break;
			case EDummyState::Runner_Duck:
				Index = 1;
				break;
			case EDummyState::Runner_Deer:
				Index = 2;
				break;
			case EDummyState::Runner_Statue:
				Index = 3;
				break;
			default:
				Index = 3;
				break;
			}
			if (PlayerCharacters.IsValidIndex(Index))
			{
				if(AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(C))
				{
					PC->Server_RequestResponseCharacter(PlayerCharacters[Index],FVector(0,-2400,1500));

					// 0.2초 후 Server_ShowUI() 실행
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(
						TimerHandle, 
						FTimerDelegate::CreateLambda([PC]()
						{
							if (PC) // Controller가 아직 유효한지 확인
							{
								PC->Server_ShowUI();
							}
						}),
						0.2f, 
						false // 반복 실행 여부 (false = 한 번만 실행)
					);
				}
			}
		}
	}

	SetMaxGamePlayerCount(GetCurrentPlayerCount());
	StartMatch();
}

void APlayGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	UE_LOG(LogTemp, Log, TEXT("InitSeamlessTravelPlayerStart"));

	Super::InitSeamlessTravelPlayer(NewController);

}


void APlayGameMode::AllowCharacterSelection(APlayerController* NewPlayer)
{
	if (NewPlayer)
	{
		AJMSGamePlayController* PC = Cast<AJMSGamePlayController>(NewPlayer);
		if (PC)
		{
			if (PC->GetPawn())
			{
				PC->GetPawn()->Destroy();
			}

			if (DefaultPawnClass)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = PC;
				SpawnParams.Instigator = PC->GetPawn();

				// 플레이어 시작 위치 가져오기
				FVector SpawnLocation = FVector::ZeroVector;
				FRotator SpawnRotation = FRotator::ZeroRotator;


				FString IncomingName = TEXT("StartPosition");
				// If incoming start is specified, then just use it
				if (!IncomingName.IsEmpty())
				{
					const FName IncomingPlayerStartTag = FName(*IncomingName);
					for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
					{
						APlayerStart* Start = *It;
						if (Start && Start->PlayerStartTag == IncomingPlayerStartTag)
						{
							UE_LOG(LogTemp, Warning, TEXT("PlayerStart 태그도 찾기성공."));

							SpawnLocation = Start->GetActorLocation();
							SpawnRotation = Start->GetActorRotation();
							break; // 첫 번째 찾은 PlayerStart 사용
						}
					}
				}


				// PlayerStart를 찾지 못하면 기본 위치 사용
				if (SpawnLocation.IsZero())
				{
					UE_LOG(LogTemp, Warning, TEXT("SpawnPoint 태그가 있는 PlayerStart를 찾지 못함. 기본 위치 사용."));
					SpawnLocation = FVector(0, 0, 300); // 기본 스폰 위치
				}


				// Pawn 스폰
				APawn* TempPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams);

				if (TempPawn)
				{
					// 플레이어가 새 Pawn을 조종하도록 설정
					PC->Possess(TempPawn);
					UE_LOG(LogTemp, Warning, TEXT("새로운 임시 캐릭터를 스폰하고 Possess 완료!"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Pawn 스폰 실패!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("DefaultPawnClass가 설정되지 않음!"));
			}
		}
	}
}

int32 APlayGameMode::GetCurrentPlayerCount()
{
	return GetNumPlayers();
}
