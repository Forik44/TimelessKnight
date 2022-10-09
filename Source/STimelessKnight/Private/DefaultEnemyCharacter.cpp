// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultEnemyCharacter.h"
#include "DefaultAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ADefaultEnemyCharacter::ADefaultEnemyCharacter()
	:
MaxWalkSpeed(300)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ADefaultAIController::StaticClass();
}

// Called when the game starts or when spawned
void ADefaultEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	
}

// Called every frame
void ADefaultEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADefaultEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
