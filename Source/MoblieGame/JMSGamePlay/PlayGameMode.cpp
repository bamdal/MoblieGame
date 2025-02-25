// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"

#include "EngineUtils.h"
#include "JMSGamePlayController.h"
#include "GameFramework/PlayerStart.h"


void APlayGameMode::StartPlay()
{
	// 처음 시작시 대기상태
	bDelayedStart = true;
	Super::StartPlay();
}

void APlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	
	Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Warning, TEXT("현재 MatchState : %s "),*GetMatchState().ToString());
	if (HasMatchStarted())
	{
		// 게임 시작중 진입하면 스펙터로 전환
		NewPlayer->StartSpectatingOnly();
        UE_LOG(LogTemp, Warning, TEXT("새 플레이어 %s 가 Spectator로 설정됨."),*NewPlayer->GetName());
	}
	else
	{
		// 로비에서 대기 중이라면 캐릭터 선택 가능
		AllowCharacterSelection(NewPlayer);
		UE_LOG(LogTemp, Warning, TEXT("새 플레이어가 캐릭터를 선택할 수 있음."));
	}
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
				PC->GetPawn()->Destroyed();
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
							break;  // 첫 번째 찾은 PlayerStart 사용
						}
					}
				}
				
	
				
				// PlayerStart를 찾지 못하면 기본 위치 사용
				if (SpawnLocation.IsZero())
				{
					UE_LOG(LogTemp, Warning, TEXT("SpawnPoint 태그가 있는 PlayerStart를 찾지 못함. 기본 위치 사용."));
					SpawnLocation = FVector(0, 0, 300);  // 기본 스폰 위치
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
