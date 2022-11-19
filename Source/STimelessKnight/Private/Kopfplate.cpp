

#include "Kopfplate.h"
#include "../FPCharacter.h"

AKopfplate::AKopfplate()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Press"));
	
}

void AKopfplate::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKopfplate::OverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AKopfplate::OverlapEnd);
	
}


void AKopfplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AKopfplate::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AKopfplate::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
