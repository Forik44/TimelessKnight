// Fill out your copyright notice in the Description page of Project Settings.
#include "RegenerationField.h"
#include "../FPCharacter.h"


// Sets default values
ARegenerationField::ARegenerationField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RegenerationField"));
}

void ARegenerationField::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARegenerationField::OverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARegenerationField::OverlapEnd);
}

void ARegenerationField::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPCharacter* Character = Cast<AFPCharacter>(OtherActor);
	
	if (Character) {
		switch (FieldIndex)
		{
		case 1:
			DefaultHelthRegenerationRate = Character->XPRegenerationRate;
			Character->XPRegenerationRate = Character->XPRegenerationRate * PowerRegeneration;
			break;
		case 2:
			DefaultManaRegenerationRate = Character->ManaRegenerationRate;
			Character->ManaRegenerationRate = Character->ManaRegenerationRate * PowerRegeneration;
			break;
		default:
			break;
		}
	}

}

void ARegenerationField::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFPCharacter* Character = Cast<AFPCharacter>(OtherActor);

	if (Character) {
		switch (FieldIndex)
		{
		case 1:
			Character->XPRegenerationRate = DefaultHelthRegenerationRate;
			break;
		case 2:
			Character->ManaRegenerationRate = DefaultManaRegenerationRate;
			break;
		default:
			break;
		}
	}
}

// Called when the game starts or when spawned


// Called every frame
void ARegenerationField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

