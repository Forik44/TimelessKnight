// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimeSystemCharacterComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TargetPoint.h"
#include "SlowEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class STIMELESSKNIGHT_API ASlowEnemyCharacter : public ADefaultEnemyCharacter
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking;

	UFUNCTION(BlueprintCallable)
	void LaunchAttack();
};
