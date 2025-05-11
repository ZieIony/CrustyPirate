#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include <Camera/CameraComponent.h>
#include <Components/BoxComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <InputAction.h>
#include <InputMappingContext.h>

#include "CollectibleItem.h"
#include "MyGameInstance.h"
#include "PlayerHUD.h"
#include <Engine/TimerHandle.h>
#include <PaperZDAnimInstance.h>
#include <Sound/SoundBase.h>

#include "Captain.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API ACaptain : public APaperZDCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

#pragma region actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* AttackCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* QuitAction;
#pragma endregion

#pragma region animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AttackAnimSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* Attack2AnimSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* Attack3AnimSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AirAttackAnimSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperZDAnimSequence* AirAttack2AnimSequence;
#pragma endregion

	bool canMove = true;
	bool canAttack = true;
	bool isStunned = false;
	bool isActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int AttackDamage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackStunDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackStunForce = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHitPoints = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHitPoints = MaxHitPoints;

	FZDOnAnimationOverrideEndSignature OnAttackOverrideEndDelegate;
	FTimerHandle stunTimer;

	FTimerHandle restartTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerHUD* PlayerHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMyGameInstance* MyGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* CollectItemSound;

	ACaptain();

	virtual void BeginPlay() override;

	virtual void Tick(float dt) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool getIsAlive();

	void move(const FInputActionValue& value);

	void updateDirection(float moveDirection);

	void jumpStarted(const FInputActionValue& value);

	void jumpEnded(const FInputActionValue& value);

	void attack(const FInputActionValue& value);

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

	void takeDamage(int damageAmount, float stunDuration, float stunForce, AActor* otherActor);

	void updateHP(int newHP);

	void stun(float durationInSeconds);

	void onStunTimerTimeout();

	void onRestartTimerTimeout();

	bool tryCollectItem(ACollectibleItem& item);

	void unlockDoubleJump();

	UFUNCTION(BlueprintCallable)
	void deactivate();

	void quitGame();
};
