// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetPatrolPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTT_GetPatrolPoint::UBTT_GetPatrolPoint()
{
	
}

EBTNodeResult::Type UBTT_GetPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	FVector Result;
	TArray<ATargetPoint*> TargetPointsArray = Cast<ADefaultEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn())->TargetPoints;

	if (TargetPointsArray.Num() > 0)
	{	
		UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		int32 currentIndex = MyBlackboard->GetValueAsInt(FName("CurrentPoint"));
		Result = TargetPointsArray[currentIndex]->GetActorLocation();
		if (TargetPointsArray.Num() - 1 > currentIndex)
		{
			MyBlackboard->SetValueAsInt(FName("CurrentPoint"), ++currentIndex);
		}
		else 
		{
			MyBlackboard->SetValueAsInt(FName("CurrentPoint"), 0);
		}
		MyBlackboard->SetValueAsVector(FName("RandomLocation"), Result);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;

	}
}


