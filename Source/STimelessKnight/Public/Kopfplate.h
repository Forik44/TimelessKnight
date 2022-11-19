// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kopfplate.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlatePress);

UCLASS()
class STIMELESSKNIGHT_API AKopfplate : public AActor
{
	GENERATED_BODY()
	
public:	
	AKopfplate();

	//UPROPERTY(BlueprintAssignable)
	//FPlatePress OnPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	int SpeedPress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollision;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

};
