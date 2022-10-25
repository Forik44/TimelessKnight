// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Containers/Array.h"
#include "DefaultEnemyCharacter.h"
#include "BTT_GetPatrolPoint.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskStarted);

UCLASS()
class STIMELESSKNIGHT_API UBTT_GetPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTT_GetPatrolPoint();

	

public:

	

	virtual EBTNodeResult::Type	ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CurrentPoint;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key;
};
