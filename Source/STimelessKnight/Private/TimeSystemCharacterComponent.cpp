#include "TimeSystemCharacterComponent.h"

UTimeSystemCharacterComponent::UTimeSystemCharacterComponent()
	:
	TimeRes(10000)
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UTimeSystemCharacterComponent::BeginPlay()
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


void UTimeSystemCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UTimeSystemCharacterComponent::StartRevers()
{
	this->IsReverse = true;
}

void UTimeSystemCharacterComponent::StopRevers()
{
	this->IsReverse = false;
}

void UTimeSystemCharacterComponent::Pop()
{
	if (!Empty()) {
		FTransform CurrentTransform = TransformBuffer->GetData()[CurrentPosition];
		FTransform CurrentPhysics = PhysicsBuffer->GetData()[CurrentPosition];

		CurrentPosition = ((CurrentPosition - 1) + TimeRes) % TimeRes;
		ActiveElem--;

		GetOwner()->SetActorTransform(CurrentTransform);


		Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()))->
			SetPhysicsLinearVelocity(CurrentPhysics.GetLocation());
		Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()))->
			SetPhysicsAngularVelocity(CurrentPhysics.GetScale3D());
	}
	else {

	}

}

void UTimeSystemCharacterComponent::Push()
{
	FTransform CurrentTransform;
	CurrentTransform = GetOwner()->GetActorTransform();

	FTransform* TransformData = TransformBuffer->GetData();

	FVector LinearVelocity = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()))->
		GetPhysicsLinearVelocity();
	FVector AngularVelocity = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()))->
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

bool UTimeSystemCharacterComponent::Fully()
{
	return (ActiveElem == TimeRes) ? true : false;
}

bool UTimeSystemCharacterComponent::Empty()
{
	return (ActiveElem == 0) ? true : false;
}

