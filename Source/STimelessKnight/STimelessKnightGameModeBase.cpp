// Copyright Epic Games, Inc. All Rights Reserved.


#include "STimelessKnightGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ASTimelessKnightGameModeBase::BeginPlay()
{

	Player = Cast<AFPCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Player->OnDeath.AddDynamic(this, &ASTimelessKnightGameModeBase::EndGame);
}

void ASTimelessKnightGameModeBase::EndGame()
{
	Player->Destroy();
	OnEndGame.Broadcast();
}
