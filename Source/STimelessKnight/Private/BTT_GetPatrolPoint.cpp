// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetPatrolPoint.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_GetPatrolPoint::UBTT_GetPatrolPoint()
{
}

EBTNodeResult::Type UBTT_GetPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return EBTNodeResult::Type();

	FVector Result;
	/*Result = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), Radius);*/
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}

	bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), Result, Radius);

	if (bSuccess)
	{
		UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		MyBlackboard->ClearValue(Key.SelectedKeyName);
		MyBlackboard->SetValueAsVector(Key.SelectedKeyName, Result);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;

	}

}


