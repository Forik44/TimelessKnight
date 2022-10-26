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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXPChangedEvent, int, CurrentValue);

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
	int MaxXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int XPRegenerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int ManaRegenerationRate;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ChangeXP(int value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ChangeMana(int value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int GetXP();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int GetMana();

	UFUNCTION(BlueprintCallable, Category = "Revers")
	void ChangeManaRTOtherObject(FTimerHandle Timer, int ManaRTOtherObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int ManaRTEverything;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int ManaRTObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int ManaRTEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int ManaRTYour;

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

	int CurrentXP, CurrentMana;

	void ChangeManaRTEverything();
	void ChangeManaRTObject();
	void ChangeManaRTYour();
	void ManaRegeneration();
	void XPRegeneration();
};