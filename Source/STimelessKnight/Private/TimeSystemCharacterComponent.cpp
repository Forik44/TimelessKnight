#include "TimeSystemCharacterComponent.h"
#include "DefaultEnemyCharacter.h"
#include "SlowEnemyCharacter.h"
#include "FastEnemyCharacter.h"

UTimeSystemCharacterComponent::UTimeSystemCharacterComponent()
	:
	TimeRes(1000),
	TimeRememberRate(0.01f),
	IsReverse(false)
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

	IsAttackingBuffer = new TArray<bool>;
	IsAttackingBuffer->SetNum(TimeRes);

	IsScreamingBuffer = new TArray<bool>;
	IsScreamingBuffer->SetNum(TimeRes);

	IsRunningBuffer = new TArray<bool>;
	IsRunningBuffer->SetNum(TimeRes);

	IsCrowlingBuffer = new TArray<bool>;
	IsCrowlingBuffer->SetNum(TimeRes);

	IsStayingBuffer = new TArray<bool>;
	IsStayingBuffer->SetNum(TimeRes);

	IsFallingBuffer = new TArray<bool>;
	IsFallingBuffer->SetNum(TimeRes);

	SpeedBuffer = new TArray<float>;
	SpeedBuffer->SetNum(TimeRes);

	UNiagaraComponent* Particle = Cast<UNiagaraComponent>(GetOwner()->GetComponentByClass(UNiagaraComponent::StaticClass()));
	if (Particle)
		Particle->Deactivate();

	ActiveElem = 0;
	CurrentPosition = -1;
	

}


void UTimeSystemCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RememberState();
}

void UTimeSystemCharacterComponent::StartRevers()
{
	IsReverse = true;
	UNiagaraComponent* Particle = Cast<UNiagaraComponent>(GetOwner()->GetComponentByClass(UNiagaraComponent::StaticClass()));
	if (Particle)
		Particle->Activate();
	AFastEnemyCharacter* FastEnemy = Cast<AFastEnemyCharacter>(GetOwner());
	
}

void UTimeSystemCharacterComponent::StopRevers()
{
	IsReverse = false;
	UNiagaraComponent* Particle = Cast<UNiagaraComponent>(GetOwner()->GetComponentByClass(UNiagaraComponent::StaticClass()));
	if (Particle)
		Particle->Deactivate();
}

bool UTimeSystemCharacterComponent::GetIsReverse()
{
	return IsReverse;
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
		ASlowEnemyCharacter* SlowEnemy = Cast<ASlowEnemyCharacter>(GetOwner());
		if (SlowEnemy)
		{
			bool IsAttacking = IsAttackingBuffer->GetData()[CurrentPosition];
			SlowEnemy->IsAttacking = IsAttacking;
		}
		AFastEnemyCharacter* FastEnemy = Cast<AFastEnemyCharacter>(GetOwner());
		if (FastEnemy)
		{
			bool IsAttacking = IsAttackingBuffer->GetData()[CurrentPosition];
			bool IsScreaming = IsScreamingBuffer->GetData()[CurrentPosition];
			bool IsRunning = IsRunningBuffer->GetData()[CurrentPosition];
			bool IsCrowling = IsCrowlingBuffer->GetData()[CurrentPosition];
			
			bool IsStaying = IsStayingBuffer->GetData()[CurrentPosition];
			bool IsFalling = IsFallingBuffer->GetData()[CurrentPosition];
			float Speed = SpeedBuffer->GetData()[CurrentPosition];
			FastEnemy->SetIsAttacking(IsAttacking);
			FastEnemy->SetIsScreaming(IsScreaming);
			FastEnemy->SetIsRunning(IsRunning);
			FastEnemy->SetIsCrowling(IsCrowling);
			FastEnemy->SetIsStaying(IsStaying);
			FastEnemy->SetIsFalling(IsFalling);
			FastEnemy->SetMaxWalkSpeed(Speed);
		}
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

	bool* IsAttackingData = IsAttackingBuffer->GetData();
	bool* IsScreamingData = IsScreamingBuffer->GetData();
	bool* IsRunningData = IsRunningBuffer->GetData();
	bool* IsCrowlingData = IsCrowlingBuffer->GetData();

	bool* IsStayingData = IsStayingBuffer->GetData();
	bool* IsFallingData = IsFallingBuffer->GetData();
	float* SpeedData = SpeedBuffer->GetData();
	CurrentPosition = (++CurrentPosition) % TimeRes;
	PhysicsData[CurrentPosition] = CurrentPhysics;
	TransformData[CurrentPosition] = CurrentTransform;

	ASlowEnemyCharacter* SlowEnemy = Cast<ASlowEnemyCharacter>(GetOwner());
	if (SlowEnemy)
	{
		IsAttackingData[CurrentPosition] = SlowEnemy->IsAttacking;
	}

	AFastEnemyCharacter* FastEnemy = Cast<AFastEnemyCharacter>(GetOwner());
	if (FastEnemy)
	{
		IsAttackingData[CurrentPosition] = FastEnemy->GetIsAttacking();
		IsScreamingData[CurrentPosition] = FastEnemy->GetIsScreaming();
		IsRunningData[CurrentPosition] = FastEnemy->GetIsRunning();
		IsCrowlingData[CurrentPosition] = FastEnemy->GetIsCrowling();
		IsStayingData[CurrentPosition] = FastEnemy->GetIsStaying();
		IsFallingData[CurrentPosition] = FastEnemy->GetIsFalling();
		SpeedData[CurrentPosition] = FastEnemy->GetMaxWalkSpeed();
	}

	if (ActiveElem < TimeRes) 
	{
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

void UTimeSystemCharacterComponent::StartRememberState()
{
	GetWorld()->GetTimerManager().SetTimer(RememberTimer, this, &UTimeSystemCharacterComponent::RememberState, TimeRememberRate, true);
}

void UTimeSystemCharacterComponent::StopRememberState()
{
	GetWorld()->GetTimerManager().ClearTimer(RememberTimer);
}

void UTimeSystemCharacterComponent::RememberState()
{
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

