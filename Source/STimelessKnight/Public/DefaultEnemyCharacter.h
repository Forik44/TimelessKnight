// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimeSystemCharacterComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TargetPoint.h"
#include "Niagara/Public/NiagaraComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ATargetPoint*> TargetPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimeSystemCharacterComponent* TimeSystemCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* DefaultSound;

	UFUNCTION(BlueprintCallable)
	void SetMaxWalkSpeed(float value);

	UFUNCTION(BlueprintCallable)
	float GetMaxWalkSpeed();


};
