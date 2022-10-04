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
}

void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPCharacter::Tick(float DeltaTime)
{
	
}

void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Hori", this, &AFPCharacter::HoriMove);
	InputComponent->BindAxis("Vert", this, &AFPCharacter::VertMove);

	InputComponent->BindAxis("HoriRot", this, &AFPCharacter::HoriRot);
	InputComponent->BindAxis("VertRot", this, &AFPCharacter::VertRot);

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

