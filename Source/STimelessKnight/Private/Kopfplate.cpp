

#include "Kopfplate.h"
#include "../FPCharacter.h"

AKopfplate::AKopfplate()
{
	PrimaryActorTick.bCanEverTick = true;
	Plate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Press"));
	
}

void AKopfplate::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AKopfplate::OverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AKopfplate::OverlapEnd);
	
	StartLocation = Plate->GetComponentLocation();
	ChangedLocation = FVector(Plate->GetComponentLocation().X, Plate->GetComponentLocation().Y, Plate->GetComponentLocation().Z - Stroke*Plate->GetComponentScale().Z);

}


void AKopfplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AKopfplate::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnKopfplatePressed.Broadcast();
}

void AKopfplate::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnKopfplateReleased.Broadcast();
}
