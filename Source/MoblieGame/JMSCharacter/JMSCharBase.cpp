#include "JMSCharBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MoblieGame/JMSGamePlay/JMSGamePlayController.h"

AJMSCharBase::AJMSCharBase()
{
	PrimaryActorTick.bCanEverTick = true;

	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -89.0f));
}


void AJMSCharBase::BeginPlay()
{
	Super::BeginPlay();
	CameraBoom->ProbeSize = 12.0f;
	ApplyInputMappingContext();
}

void AJMSCharBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ApplyInputMappingContext();
}


void AJMSCharBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AJMSCharBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 1) IMC 세팅
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Asset, 0);
		}
	}

	// 2) IA 입력 액션 세팅
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Looking
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AJMSCharBase::Look);

		// Jump
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AJMSCharBase::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &AJMSCharBase::StopJumping);

		// Move
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AJMSCharBase::Move);

		// ESC
		EnhancedInputComponent->BindAction(IA_ESC, ETriggerEvent::Started, this, &AJMSCharBase::ExitGame);

		// PlayStart
		EnhancedInputComponent->BindAction(IA_PlayStart, ETriggerEvent::Started, this, &AJMSCharBase::PlayStart);
	}
}

void AJMSCharBase::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJMSCharBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForWardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForWardDirection, MovementAxisVector.Y);
		AddMovementInput(RightDirection, MovementAxisVector.X);
	}
}

void AJMSCharBase::ExitGame(const FInputActionValue& InputActionValue)
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void AJMSCharBase::PlayStart(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Display, TEXT("Play Start"));
}

void AJMSCharBase::ApplyInputMappingContext()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		if (InputSubsystem)
		{
			// 기존 IMC 제거
			InputSubsystem->ClearAllMappings();

			// 올바른 IMC 적용 (BP에서 설정한 값 사용)
			if (IMC_Asset)
			{
				InputSubsystem->AddMappingContext(IMC_Asset, 0);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s: IMC is NULL!"), *GetName());
			}
		}
	}
}
