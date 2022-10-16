#include "FPCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "STimelessKnightGameModeBase.h"

AFPCharacter::AFPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 40));

	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 12.f;
}

void AFPCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f) 
	{
		return;
	}
	float StartBaseEyeHight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHight - BaseEyeHeight + HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0, 0, BaseEyeHeight), false);
}
void AFPCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f) 
	{
		return;
	}
	float StartBaseEyeHight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	CrouchEyeOffset.Z += StartBaseEyeHight - BaseEyeHeight - HalfHeightAdjust;
	Camera->SetRelativeLocation(FVector(0, 0, BaseEyeHeight), false);
}
void AFPCharacter::CalcCamera(float Deltatime, struct FMinimalVievInfo& OutResult)
{
	if (Camera)
	{
		Camera->GetCameraView(Deltatime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovementComponent()->MaxWalkSpeed = 170.0f;
}

void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.f - CrouchInterpTime) * CrouchEyeOffset;
}

void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Hori", this, &AFPCharacter::HoriMove);
	PlayerInputComponent->BindAxis("Vert", this, &AFPCharacter::VertMove);

	PlayerInputComponent->BindAxis("HoriRot", this, &AFPCharacter::HoriRot);
	PlayerInputComponent->BindAxis("VertRot", this, &AFPCharacter::VertRot);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFPCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AFPCharacter::StopRun);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::Jump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPCharacter::StopCrouch);

	PlayerInputComponent->BindAction("RewindTimeYour", IE_Pressed, this, &AFPCharacter::RewindTimeYour);
	PlayerInputComponent->BindAction("RewindTimeObject", IE_Pressed, this, &AFPCharacter::RewindTimeObject);
	PlayerInputComponent->BindAction("RewindTimeEverything", IE_Pressed, this, &AFPCharacter::RewindTimeEverything);
}

void AFPCharacter::HoriMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AFPCharacter::VertMove(float value)
{
	IsVertMove = value > 0.0f;
	if (value)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFPCharacter::HoriRot(float value)
{
	if (value)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}

void AFPCharacter::VertRot(float value)
{
	if (value)
	{
		float Rotation = Camera->GetRelativeRotation().Pitch + value;

		if (Rotation < 80 && Rotation > -80)
		{
			Camera->AddLocalRotation(FRotator(value, 0, 0));
		}
	}
}



APawn* AFPCharacter::GetPlayerPawn() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) return nullptr;
	return PlayerPawn;
}
UCharacterMovementComponent* AFPCharacter::GetCharacterMovementComponent() const
{
	UCharacterMovementComponent* CharacterMovementComponent = Cast <UCharacterMovementComponent>(GetPlayerPawn()->GetMovementComponent());
	if (!CharacterMovementComponent) return nullptr;
	return CharacterMovementComponent;
}
void AFPCharacter::StartRun()
{
	if (IsVertMove) GetCharacterMovementComponent()->MaxWalkSpeed = 300.0f;
}
void AFPCharacter::StopRun()
{
	GetCharacterMovementComponent()->MaxWalkSpeed = 170.0f;
}

void AFPCharacter::StartCrouch()
{
	Crouch();
}
void AFPCharacter::StopCrouch()
{
	UnCrouch();
}

void AFPCharacter::RewindTimeYour()
{
	UE_LOG(LogTemp, TEXT("rewind time on your own"));
}
void AFPCharacter::RewindTimeObject()
{
	UE_LOG(LogTemp, TEXT("rewind time on an object"));
}
void AFPCharacter::RewindTimeEverything()
{
	UE_LOG(LogTemp, TEXT("rewind time for everything"));
}