// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultEnemyCharacter.h"
#include "FastEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsAttackingChangedEvent, bool, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsScreamingChangedEvent, bool, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsRunningChangedEvent, bool, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsCrowlingChangedEvent, bool, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsStayingChangedEvent, bool, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsFallingChangedEvent, bool, CurrentValue);

/**
 * 
 */
UCLASS()
class STIMELESSKNIGHT_API AFastEnemyCharacter : public ADefaultEnemyCharacter
{
	GENERATED_BODY()


	AFastEnemyCharacter();
private:
	bool IsAttacking;
	bool IsScreaming;
	bool IsRunning;
	bool IsCrowling;
	bool IsStaying;
	bool IsFalling;

	bool ActiveCrawl;
	bool ActiveRun;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage;

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool value);

	UFUNCTION(BlueprintCallable)
	void SetIsScreaming(bool value);

	UFUNCTION(BlueprintCallable)
	void SetIsRunning(bool value);

	UFUNCTION(BlueprintCallable)
	void SetIsCrowling(bool value);

	UFUNCTION(BlueprintCallable)
	void SetIsStaying(bool value);

	UFUNCTION(BlueprintCallable)
	void SetIsFalling(bool value);

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
	bool GetIsScreaming();

	UFUNCTION(BlueprintCallable)
	bool GetIsRunning();

	UFUNCTION(BlueprintCallable)
	bool GetIsCrowling();

	UFUNCTION(BlueprintCallable)
	bool GetIsStaying();

	UFUNCTION(BlueprintCallable)
	bool GetIsFalling();

	UFUNCTION(BlueprintCallable)
	void LaunchAttack();

	UPROPERTY(BlueprintAssignable)
	FIsAttackingChangedEvent OnIsAttackingChanged;

	UPROPERTY(BlueprintAssignable)
	FIsScreamingChangedEvent OnIsScreamingChanged;

	UPROPERTY(BlueprintAssignable)
	FIsRunningChangedEvent OnIsRunningChanged;

	UPROPERTY(BlueprintAssignable)
	FIsCrowlingChangedEvent OnIsCrowlingChanged;

	UPROPERTY(BlueprintAssignable)
	FIsStayingChangedEvent OnIsStayingChanged;

	UPROPERTY(BlueprintAssignable)
	FIsFallingChangedEvent OnIsFallingChanged;
};
