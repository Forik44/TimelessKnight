// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "RegenerationField.generated.h"

UCLASS()
class STIMELESSKNIGHT_API ARegenerationField : public AActor
{
	GENERATED_BODY()
	
public:	

	ARegenerationField();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FieldIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PowerRegeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollision;


	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	float DefaultHelthRegenerationRate;
	float DefaultManaRegenerationRate;

protected:

	virtual void BeginPlay() override;

	

public:	

	virtual void Tick(float DeltaTime) override;

};
