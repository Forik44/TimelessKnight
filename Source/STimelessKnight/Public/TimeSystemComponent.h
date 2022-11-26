// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CircleBuffer.h"
#include "Math/Vector.h"
#include "Components/StaticMeshComponent.h"
#include "TimeSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STIMELESSKNIGHT_API UTimeSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTimeSystemComponent();
	

protected:
	
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere)
	int TimeRes;

	UPROPERTY(EditAnywhere)
	float TimeRememberRate;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartRevers();

	UFUNCTION(BlueprintCallable)
	void StopRevers();


	void Pop();

	void Push();

	bool Fully();

	bool Empty();

	void StartRememberState();

	void StopRememberState();
	

private:


	bool IsReverse;
	int CurrentPosition;
	int ActiveElem;

	//CircleBuffer* TransformBuffer;
	//CircleBuffer<FVector>* PhisicsBuffer;
	
	TArray<FTransform>* TransformBuffer;
	TArray<FTransform>* PhysicsBuffer;

	FTransform CurrentActorTransform;

	FTimerHandle RememberTimer;

	void RememberState();

};
