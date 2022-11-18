#include "FastEnemyCharacter.h"
#include "DefaultAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AFastEnemyCharacter::AFastEnemyCharacter()
	:
IsStaying(true),
AttackLength(150),
AttackDamage(70)
{
	
}

void AFastEnemyCharacter::SetIsAttacking(bool value)
{
	IsAttacking = value;
	OnIsAttackingChanged.Broadcast(value);
}

void AFastEnemyCharacter::SetIsScreaming(bool value)
{
	IsScreaming = value;
	OnIsScreamingChanged.Broadcast(value);
}

void AFastEnemyCharacter::SetIsRunning(bool value)
{
	IsRunning = value;
	if (value)
	{
		UE_LOG(LogTemp, Log, TEXT("TRUE Running"));
		if (!ActiveRun && !ActiveCrawl)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(32);
			GetCapsuleComponent()->SetCapsuleRadius(32);
			GetMesh()->AddRelativeLocation(FVector(-50, 0, 50));
			ActiveRun = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("False Running"));
		if (ActiveRun)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(88);
			GetCapsuleComponent()->SetCapsuleRadius(34);
			GetMesh()->AddRelativeLocation(FVector(50, 0, -50));
			ActiveRun = false;
		}
	}
	OnIsRunningChanged.Broadcast(value);
}

void AFastEnemyCharacter::SetIsCrowling(bool value)
{
	IsCrowling = value;
	if (value)
	{
		UE_LOG(LogTemp, Log, TEXT("TRUE Crowling"));
		if (ActiveRun)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(88);
			GetCapsuleComponent()->SetCapsuleRadius(34);
			GetMesh()->AddRelativeLocation(FVector(50, 0, -50));
			ActiveRun = false;
		}
		if (!ActiveCrawl)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(32);
			GetCapsuleComponent()->SetCapsuleRadius(32);
			GetMesh()->AddRelativeLocation(FVector(-50, 0, 50));
			ActiveCrawl = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("False Crowling"));
		if(ActiveCrawl)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(88);
			GetCapsuleComponent()->SetCapsuleRadius(34);
			GetMesh()->AddRelativeLocation(FVector(50, 0, -50));
			ActiveCrawl = false;
		}
	}
	OnIsCrowlingChanged.Broadcast(value);
}

void AFastEnemyCharacter::SetIsStaying(bool value)
{
	IsStaying = value;
	OnIsStayingChanged.Broadcast(value);
}

void AFastEnemyCharacter::SetIsFalling(bool value)
{
	IsFalling = value;
	OnIsFallingChanged.Broadcast(value);
}

bool AFastEnemyCharacter::GetIsAttacking()
{
	return IsAttacking;
}

bool AFastEnemyCharacter::GetIsScreaming()
{
	return IsScreaming;
}

bool AFastEnemyCharacter::GetIsRunning()
{
	return IsRunning;
}

bool AFastEnemyCharacter::GetIsCrowling()
{
	return IsCrowling;
}

bool AFastEnemyCharacter::GetIsStaying()
{
	return IsStaying;
}

bool AFastEnemyCharacter::GetIsFalling()
{
	return IsFalling;
}

void AFastEnemyCharacter::LaunchAttack()
{
	FHitResult* Hit = new FHitResult();
	FVector Start = GetActorLocation() + UKismetMathLibrary::GetForwardVector(GetActorRotation()) * 40;
	FVector End = UKismetMathLibrary::GetForwardVector(GetRootComponent()->GetComponentRotation()) * AttackLength + Start;
	bool Success = GetWorld()->LineTraceSingleByChannel(*Hit, Start, End, ECC_Pawn);
	if (Success)
	{
		AActor* ActorHited = Cast<AActor>(Hit->Actor);
		UGameplayStatics::ApplyDamage(ActorHited, AttackDamage, GetController(), this, UDamageType::StaticClass());
	}
}
