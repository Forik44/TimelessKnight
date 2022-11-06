#include "SlowEnemyCharacter.h"
#include "DefaultAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void ASlowEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASlowEnemyCharacter::LaunchAttack()
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
