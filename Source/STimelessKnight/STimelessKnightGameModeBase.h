// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPCharacter.h"
#include "STimelessKnightGameModeBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndGameEvent);
/**
 * 
 */
UCLASS()
class STIMELESSKNIGHT_API ASTimelessKnightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void EndGame();

	UPROPERTY(BlueprintAssignable)
	FEndGameEvent OnEndGame;

	AFPCharacter* Player;

};
