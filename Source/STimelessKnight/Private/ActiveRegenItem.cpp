// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveRegenItem.h"

// Sets default values
AActiveRegenItem::AActiveRegenItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AActiveRegenItem::SetCustomDeapth(bool value)
{
	Mesh->SetRenderCustomDepth(value);
}

// Called when the game starts or when spawned
void AActiveRegenItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActiveRegenItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

