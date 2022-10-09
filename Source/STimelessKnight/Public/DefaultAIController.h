// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefaultAIController.generated.h"

/**
 * 
 */
UCLASS()
class STIMELESSKNIGHT_API ADefaultAIController : public AAIController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
