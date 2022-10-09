
#include "TimeSystemComponent.h"


UTimeSystemComponent::UTimeSystemComponent()
	:
	TimeRes(10000)
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UTimeSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	TransformBuffer = new TArray<FTransform>;
	TransformBuffer->SetNum(TimeRes);


	PhysicsBuffer = new TArray<FTransform>;
	PhysicsBuffer->SetNum(TimeRes);

	ActiveElem = 0;
	CurrentPosition = -1;
	IsReverse = false;

}


void UTimeSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsReverse) 
	{
		if (!Empty()) 
		{
			Pop();
		}
		else
		{
			StopRevers();
		}
	}
	else
	{
		Push();
	}
}

void UTimeSystemComponent::StartRevers()
{
	this->IsReverse = true;
}

void UTimeSystemComponent::StopRevers()
{
	this->IsReverse = false;
}

void UTimeSystemComponent::Pop()
{
	if (!Empty()) {
		FTransform CurrentTransform = TransformBuffer->GetData()[CurrentPosition];
		FTransform CurrentPhysics = PhysicsBuffer->GetData()[CurrentPosition];

		CurrentPosition = ((CurrentPosition - 1) + TimeRes) % TimeRes;
		ActiveElem--;

		GetOwner()->SetActorTransform(CurrentTransform);
		
		
		Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))->
			SetPhysicsLinearVelocity(CurrentPhysics.GetLocation());
		Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))->
			SetPhysicsAngularVelocity(CurrentPhysics.GetScale3D());
	}
	else {
		
	}

}

void UTimeSystemComponent::Push()
{
	FTransform CurrentTransform;
	CurrentTransform = GetOwner()->GetActorTransform();

	FTransform* TransformData = TransformBuffer->GetData();

	FVector LinearVelocity = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))->
								GetPhysicsLinearVelocity();
	FVector AngularVelocity = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))->
								GetPhysicsAngularVelocity();
	FTransform CurrentPhysics = FTransform(FRotator(0, 0, 0), LinearVelocity, AngularVelocity);
	FTransform* PhysicsData = PhysicsBuffer->GetData();

	CurrentPosition = (++CurrentPosition) % TimeRes;
	PhysicsData[CurrentPosition] = CurrentPhysics;
	TransformData[CurrentPosition] = CurrentTransform;

	if (ActiveElem < TimeRes) {
		ActiveElem++;
	}

}

bool UTimeSystemComponent::Fully()
{
	return (ActiveElem == TimeRes) ? true : false;
}

bool UTimeSystemComponent::Empty()
{
	return (ActiveElem == 0) ? true : false;
}









