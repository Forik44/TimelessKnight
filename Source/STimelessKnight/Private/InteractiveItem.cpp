// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveItem.h"

// Sets default values
AInteractiveItem::AInteractiveItem()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TimeSystem = CreateDefaultSubobject<UTimeSystemComponent>(TEXT("TimeSystem"));
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleSystem"));
	ParticleSystem->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AInteractiveItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

