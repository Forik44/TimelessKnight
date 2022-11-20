// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveItem.h"

void AInteractiveItem::SetCustomDeapth(bool value)
{
	Mesh->SetRenderCustomDepth(value);
}

// Sets default values
AInteractiveItem::AInteractiveItem()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TimeSystem = CreateDefaultSubobject<UTimeSystemComponent>(TEXT("TimeSystem"));
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleSystem"));
	
}

// Called when the game starts or when spawned
void AInteractiveItem::BeginPlay()
{
	ParticleSystem->AttachTo(RootComponent);
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

