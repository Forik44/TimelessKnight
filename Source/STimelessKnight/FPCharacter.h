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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FManaChangedEvent, float, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXPChangedEvent, float, CurrentValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReversObjectPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReversObjectReleased);


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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FXPChangedEvent OnXPChanged;

	UPROPERTY(BlueprintAssignable)
	FManaChangedEvent OnManaChanged;

	UPROPERTY(BlueprintAssignable)
	FDeathEvent OnDeath;

	UPROPERTY(BlueprintAssignable)
	FReversObjectPressed OnReversObjectPressed;

	UPROPERTY(BlueprintAssignable)
	FReversObjectReleased OnReversObjectReleased;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "TimeSystem")
	UTimeSystemCharacterComponent* TimeSystemCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Crouch)
	FVector CrouchEyeOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Crouch)
	float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float XPRegenerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRegenerationRate;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ChangeXP(float value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ChangeMana(float value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetXP();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetMana();

	UFUNCTION(BlueprintCallable, Category = "Revers")
	void ChangeManaRTOtherObject(FTimerHandle Timer, float ManaRTOtherObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRTEverything;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRTObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRTEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRTYour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpeedMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpeedRegeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpeedStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpeedRun;
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
	FTimerHandle XPRegenerationTimer;
	FTimerHandle ManaRegenerationTimer;
	FTimerHandle ManaRTYourTimer;
	FTimerHandle ManaRTObjectTimer;
	FTimerHandle ManaRTEverythingTimer;

	float CurrentXP, CurrentMana;

	void ChangeManaRTEverything();
	void ChangeManaRTObject();
	void ChangeManaRTYour();
	void ManaRegeneration();
	void XPRegeneration();
};