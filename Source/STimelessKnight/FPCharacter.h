#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/InteractiveItem.h"
#include "Public/DefaultEnemyCharacter.h"
#include "Public/TimeSystemCharacterComponent.h"
#include "FPCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FManaChangedEvent, int, CurrentValue);

UCLASS()
class STIMELESSKNIGHT_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCharacter();
	
	void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void CalcCamera(float Deltatime, struct FMinimalViewInfo& OutResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
		FManaChangedEvent OnManaChanged;

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "TimeSystem")
		UTimeSystemCharacterComponent* TimeSystemCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Crouch)
		FVector CrouchEyeOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Crouch)
		float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxXP, MaxMana, ManaRegenerationRate;

	UFUNCTION(BlueprintCallable)
		void ChangeXP(int value);

	UFUNCTION(BlueprintCallable)
		void ChangeMana(int value);

	UFUNCTION(BlueprintCallable)
		int GetXP();

	UFUNCTION(BlueprintCallable)
		int GetMana();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ManaRTYour, ManaRTObject, ManaRTEverything;

private:

	AInteractiveItem* CatchedObject;
	ADefaultEnemyCharacter* CatchedEnemy;

	void HoriMove(float value);
	void VertMove(float value);

	void HoriRot(float value);
	void VertRot(float value);

	void StartRun();
	void StopRun();

	void StartCrouch();
	void StopCrouch();
	
	void RewindTimeYourStart();
	void RewindTimeYourStop();
	void RewindTimeObjectStart();
	void RewindTimeObjectStop();
	void RewindTimeEverythingStart();
	void RewindTimeEverythingStop();

	APawn* GetPlayerPawn() const;
	UCharacterMovementComponent* GetCharacterMovementComponent() const;
	bool IsVertMove;

	FTimerHandle ManaTimer;

	int CurrentXP, CurrentMana;
};