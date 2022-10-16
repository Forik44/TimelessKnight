// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeSystemCharacterComponent.h"
#include "DefaultEnemyCharacter.generated.h"

UCLASS()
class STIMELESSKNIGHT_API ADefaultEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere)
	UTimeSystemCharacterComponent* TimeSystemCharacter;

};
