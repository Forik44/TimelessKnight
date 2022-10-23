#include "FPCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "STimelessKnightGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

AFPCharacter::AFPCharacter()
	:
MaxXP(100),
MaxMana(100),
ManaRTYour(2),
ManaRTObject(3),
ManaRTEnemy(4),
ManaRTEverything(10),
ManaRegenerationRate(1),
SpeedStep(200),
SpeedRun(350)
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 40));

	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 12.f;

	TimeSystemCharacter = CreateDefaultSubobject<UTimeSystemCharacterComponent>(TEXT("TimeSystemCharacter"));


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

void AFPCharacter::CalcCamera(float Deltatime, struct FMinimalViewInfo& OutResult)
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
	GetCharacterMovementComponent()->MaxWalkSpeed = SpeedStep;
	CurrentXP = MaxXP;
	CurrentMana = MaxMana;
	GetWorld()->GetTimerManager().SetTimer(ManaRegenerationTimer, this, &AFPCharacter::ManaRegeneration, 3.0f, true);

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

	PlayerInputComponent->BindAction("RewindTimeYour", IE_Pressed, this, &AFPCharacter::RewindTimeYourStart);
	PlayerInputComponent->BindAction("RewindTimeObject", IE_Pressed, this, &AFPCharacter::RewindTimeObjectStart);
	PlayerInputComponent->BindAction("RewindTimeEverything", IE_Pressed, this, &AFPCharacter::RewindTimeEverythingStart);

	PlayerInputComponent->BindAction("RewindTimeYour", IE_Released, this, &AFPCharacter::RewindTimeYourStop);
	PlayerInputComponent->BindAction("RewindTimeObject", IE_Released, this, &AFPCharacter::RewindTimeObjectStop);
	PlayerInputComponent->BindAction("RewindTimeEverything", IE_Released, this, &AFPCharacter::RewindTimeEverythingStop);
}

void AFPCharacter::ChangeXP(int value)
{

}

void AFPCharacter::ChangeMana(int value)
{
	if ((value <= MaxMana) && (value >= 0)) CurrentMana = value;
	OnManaChanged.Broadcast(CurrentMana);
}

void AFPCharacter::ManaRegeneration()
{
	if (CurrentMana < MaxMana)
	{
		ChangeMana(CurrentMana + ManaRegenerationRate);
	}
}

void AFPCharacter::ChangeManaRTYour()
{
	if (CurrentMana >= ManaRTYour) {
		ChangeMana(CurrentMana - ManaRTYour);
		TimeSystemCharacter->StartRevers();
	}
	else
	{
		TimeSystemCharacter->StopRevers();
		GetWorld()->GetTimerManager().ClearTimer(ManaRTYourTimer);
		RewindTimeYourStop();
	}
}

void AFPCharacter::ChangeManaRTObject()
{
	if (CurrentMana >= ManaRTObject) {
		if (CatchedObject)
		{
			ChangeMana(CurrentMana - ManaRTObject);
			CatchedObject->TimeSystem->StartRevers();
		}
		if (CatchedEnemy)
		{
			ChangeMana(CurrentMana - ManaRTEnemy);
			CatchedEnemy->TimeSystemCharacter->StartRevers();
		}
	}
	else
	{
		if (CatchedObject)
		{
			CatchedObject->TimeSystem->StopRevers();
		}
		if (CatchedEnemy)
		{
			CatchedEnemy->TimeSystemCharacter->StopRevers();
		}
		RewindTimeObjectStop();
	}
}

void AFPCharacter::ChangeManaRTEverything()
{
	if (CurrentMana >= ManaRTEverything) {
		ChangeMana(CurrentMana - ManaRTEverything);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ManaRTEverythingTimer);
		RewindTimeEverythingStop();
	}
}


int AFPCharacter::GetXP()
{
	return CurrentXP;
}

int AFPCharacter::GetMana()
{
	return CurrentMana;
}

void AFPCharacter::ChangeManaRTOtherObject(FTimerHandle Timer, int ManaRTOtherObject)
{
	if (CurrentMana >= ManaRTOtherObject) {
		ChangeMana(CurrentMana - ManaRTOtherObject);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		RewindTimeObjectStop();
	}
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
	if (IsVertMove) GetCharacterMovementComponent()->MaxWalkSpeed = SpeedRun;
}

void AFPCharacter::StopRun()
{
	GetCharacterMovementComponent()->MaxWalkSpeed = SpeedStep;
}

void AFPCharacter::StartCrouch()
{
	Crouch();
}

void AFPCharacter::StopCrouch()
{
	UnCrouch();
}

void AFPCharacter::RewindTimeYourStart()
{
	GetWorld()->GetTimerManager().SetTimer(ManaRTYourTimer, this, &AFPCharacter::ChangeManaRTYour, 1.0f, true, 0);
	UE_LOG(LogTemp, Log, TEXT("rewind time on your own"));
	Camera->PostProcessSettings.VignetteIntensity = 1;
	Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
	Camera->PostProcessSettings.GrainIntensity = 0.8;
	Camera->PostProcessSettings.bOverride_GrainIntensity = true;
}

void AFPCharacter::RewindTimeYourStop()
{
	UE_LOG(LogTemp, Log, TEXT("rewind time on your own"));
	TimeSystemCharacter->StopRevers();
	Camera->PostProcessSettings.VignetteIntensity = 0;
	Camera->PostProcessSettings.bOverride_VignetteIntensity = false;
	Camera->PostProcessSettings.GrainIntensity = 0;
	Camera->PostProcessSettings.bOverride_GrainIntensity = false;
	GetWorld()->GetTimerManager().ClearTimer(ManaRTYourTimer);
}

void AFPCharacter::RewindTimeObjectStart()
{
	UE_LOG(LogTemp, Log, TEXT("rewind time on an object start"));
	OnReversObjectPressed.Broadcast();

	FHitResult* Hit = new FHitResult();
	FVector Start = Camera->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * 50;
	FVector End = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * 1000 + Start;
	GetWorld()->LineTraceSingleByChannel(*Hit, Start, End, ECC_Visibility);

	CatchedObject = Cast<AInteractiveItem>(Hit->Actor);
	if (CatchedObject)
	{
		GetWorld()->GetTimerManager().SetTimer(ManaRTObjectTimer, this, &AFPCharacter::ChangeManaRTObject, 1.0f, true, 0);
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(*Hit, Start, End, ECC_Pawn);
		CatchedEnemy = Cast<ADefaultEnemyCharacter>(Hit->Actor);
		if (CatchedEnemy)
		{
			UE_LOG(LogTemp, Log, TEXT("Ob"));
			GetWorld()->GetTimerManager().SetTimer(ManaRTObjectTimer, this, &AFPCharacter::ChangeManaRTObject, 1.0f, true, 0);
		}
	}
}
void AFPCharacter::RewindTimeObjectStop()
{
	UE_LOG(LogTemp, Log, TEXT("rewind time on an object stoped"));
	OnReversObjectReleased.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(ManaRTObjectTimer);
	if (CatchedObject)
	{
		CatchedObject->TimeSystem->StopRevers();
		CatchedObject = nullptr;
	}
	if (CatchedEnemy)
	{
		CatchedEnemy->TimeSystemCharacter->StopRevers();
		CatchedEnemy = nullptr;
	}
}

void AFPCharacter::RewindTimeEverythingStart()
{
	if (CurrentMana >= ManaRTEverything)
	{
		GetWorld()->GetTimerManager().SetTimer(ManaRTEverythingTimer, this, &AFPCharacter::ChangeManaRTEverything, 1.0f, true, 0);
		UE_LOG(LogTemp, Log, TEXT("rewind time for everything"));
	}
}
void AFPCharacter::RewindTimeEverythingStop()
{

}