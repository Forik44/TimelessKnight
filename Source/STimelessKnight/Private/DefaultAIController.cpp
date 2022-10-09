// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAIController.h"
#include "BehaviorTree/BehaviorTree.h"

void ADefaultAIController::BeginPlay()
{
	UBehaviorTree* BT_SlowMain = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/AI/BehaviorTrees/BT_SlowMain"));
	if (BT_SlowMain)
	{
		RunBehaviorTree(BT_SlowMain);
	}
}
