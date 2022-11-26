#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Vector.h"
#include "Components/CapsuleComponent.h"
#include "TimeSystemCharacterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STIMELESSKNIGHT_API UTimeSystemCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTimeSystemCharacterComponent();

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

	UFUNCTION(BlueprintCallable)
		bool GetIsReverse();

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

	TArray<FTransform>* TransformBuffer;
	TArray<FTransform>* PhysicsBuffer;
	TArray<bool>* IsAttackingBuffer;
	TArray<bool>* IsScreamingBuffer;
	TArray<bool>* IsRunningBuffer;
	TArray<bool>* IsCrowlingBuffer;
	TArray<bool>* IsStayingBuffer;
	TArray<bool>* IsFallingBuffer;
	TArray<float>* SpeedBuffer;

	FTimerHandle RememberTimer;

	void RememberState();
};
