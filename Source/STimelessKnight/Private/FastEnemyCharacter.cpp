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
	OnIsRunningChanged.Broadcast(value);
}

void AFastEnemyCharacter::SetIsCrowling(bool value)
{
	IsCrowling = value;
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
