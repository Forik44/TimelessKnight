// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TimeSystemComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "InteractiveItem.generated.h"


UCLASS()
class STIMELESSKNIGHT_API AInteractiveItem : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimeSystemComponent* TimeSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* ParticleSystem;

	UFUNCTION(BlueprintCallable, Category = "Interactive")
	void SetCustomDeapth(bool value);


	AInteractiveItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
