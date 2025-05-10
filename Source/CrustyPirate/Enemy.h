// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Captain.h"
#include "CoreMinimal.h"
#include <Delegates/DelegateCombinations.h>

#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

class USphereComponent;
class UBoxComponent;
class ACaptain;
class UPaperZDAnimSequence;

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* PlayerDetectorSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* AttackCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ACaptain* FollowTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AttackAnimSequence;

	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChangedEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float stopDistanceToTarget = 70;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackCooldownInSeconds = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHitPoints = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int hitPoints = MaxHitPoints;

	bool canMove = true;
	bool isStunned = false;
	bool canAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int attackDamage = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackStunDuration = 0.3f;

	FTimerHandle stunTimer;
	FTimerHandle attackCooldownTimer;

	AEnemy();

	virtual void BeginPlay() override;

	virtual void Tick(float dt) override;

	UFUNCTION()
	void DetectorOverlapBegin(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComponent,
		int32 otherBodyIndex,
		bool fromSweep,
		const FHitResult& sweepResults
	);

	UFUNCTION()
	void DetectorOverlapEnd(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComponent,
		int32 otherBodyIndex
	);

	bool getIsAlive();

	void updateDirection(float moveDirection);

	void updateHP(int newHP);

	void takeDamage(int damageAmount, float stunDuration, float stunForce);

	void stun(float durationInSeconds);

	void onStunTimerTimeout();

	void attack();

	void onAttackCooldownTimerTimeout();

	void onAttackOverrideAnimEnd(bool completed);

	UFUNCTION()
	void AttackBoxOverlapBegin(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComponent,
		int32 otherBodyIndex,
		bool fromSweep,
		const FHitResult& sweepResults
	);

	UFUNCTION(BlueprintCallable)
	void EnableAttackCollisionBox(bool enable);
};
