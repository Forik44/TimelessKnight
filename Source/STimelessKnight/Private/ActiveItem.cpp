#include "ActiveItem.h"

AActiveItem::AActiveItem()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	PrimaryActorTick.bCanEverTick = true;

}

void AActiveItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

