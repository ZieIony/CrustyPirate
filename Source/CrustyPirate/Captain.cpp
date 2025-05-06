#include "Captain.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

ACaptain::ACaptain() {
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void ACaptain::BeginPlay() {
	Super::BeginPlay();

	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (playerController) {
		auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			playerController->GetLocalPlayer()
		);
		if (subsystem) {
			subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	OnAttackOverrideEndDelegate.BindUObject(this, &ACaptain::onAttackOverrideAnimEnd);
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACaptain::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (MyGameInstance) {
		hitPoints = MyGameInstance->PlayerHP;
		if (MyGameInstance->IsDoubleJumpUnlocked) {
			unlockDoubleJump();
		}
	}

	if (PlayerHUDClass) {
		PlayerHUDWidget = CreateWidget<UPlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PlayerHUDClass);
		if (PlayerHUDWidget) {
			PlayerHUDWidget->AddToPlayerScreen();
			PlayerHUDWidget->setHP(hitPoints);
			PlayerHUDWidget->setDiamonds(MyGameInstance->CollectedDiamondCount);
			PlayerHUDWidget->setLevel(MyGameInstance->CurrentLevelIndex);
		}
	}
}

void ACaptain::Tick(float dt) {
	Super::Tick(dt);
}

void ACaptain::SetupPlayerInputComponent(UInputComponent* playerInputComponent) {
	Super::SetupPlayerInputComponent(playerInputComponent);

	auto* enhancedInputComponent = Cast<UEnhancedInputComponent>(playerInputComponent);
	if (enhancedInputComponent) {
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACaptain::move);

		enhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ACaptain::attack);

		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACaptain::jumpStarted);
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACaptain::jumpEnded);
		enhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ACaptain::jumpEnded);

		enhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started, this, &ACaptain::quitGame);
	} else {
		throw std::exception("enhancedInputComponent is null");
	}
}

void ACaptain::move(const FInputActionValue& value) {
	float moveActionValue = value.Get<float>();
	if (isAlive && canMove && !isStunned) {
		FVector dir = { 1,0,0 };
		AddMovementInput(dir, moveActionValue);
		updateDirection(moveActionValue);
	}
}

void ACaptain::updateDirection(float moveDirection) {
	FRotator currentRotation = Controller->GetControlRotation();
	if (moveDirection < 0) {
		Controller->SetControlRotation({ currentRotation.Pitch,180,currentRotation.Roll });
	} else if (moveDirection > 0) {
		Controller->SetControlRotation({ currentRotation.Pitch,0,currentRotation.Roll });
	}
}

void ACaptain::jumpStarted(const FInputActionValue& value) {
	if (isAlive && canMove && !isStunned)
		Jump();
}

void ACaptain::jumpEnded(const FInputActionValue& value) {
	StopJumping();
}

void ACaptain::attack(const FInputActionValue& value) {
	if (isAlive && canAttack && !isStunned) {
		canAttack = false;
		canMove = false;

		if (GetVelocity().Z != 0) {
			GetAnimInstance()->PlayAnimationOverride(
				AirAttackAnimSequence, FName("AttackSlot"), 1, 0, OnAttackOverrideEndDelegate
			);
		} else {
			GetAnimInstance()->PlayAnimationOverride(
				AttackAnimSequence, FName("AttackSlot"), 1, 0, OnAttackOverrideEndDelegate
			);
		}
	}
}

void ACaptain::onAttackOverrideAnimEnd(bool completed) {
	if (isActive && isAlive) {
		canAttack = true;
		canMove = true;
	}
}

void ACaptain::AttackBoxOverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResults) {
	auto enemy = Cast<AEnemy>(otherActor);
	if (enemy) {
		enemy->takeDamage(attackDamage, attackStunDuration);
	}
}

void ACaptain::EnableAttackCollisionBox(bool enable) {
	if (enable) {
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	} else {
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

void ACaptain::takeDamage(int damageAmount, float stunDuration) {
	if (!isAlive || !isActive)
		return;

	EnableAttackCollisionBox(false);
	updateHP(std::max(0, hitPoints - damageAmount));

	if (hitPoints == 0) {
		isAlive = false;
		canMove = false;
		canAttack = false;

		float restartDelay = 3;
		GetWorldTimerManager().SetTimer(restartTimer, this, &ACaptain::onRestartTimerTimeout, 1, false, restartDelay);

		GetAnimInstance()->JumpToNode(FName("JumpDie"), FName("CaptainStateMachine"));
	} else {
		stun(stunDuration);
		GetAnimInstance()->JumpToNode(FName("JumpTakeHit"), FName("CaptainStateMachine"));
	}
}

void ACaptain::updateHP(int newHP) {
	hitPoints = newHP;
	MyGameInstance->setPlayerHP(hitPoints);
	if (PlayerHUDWidget) {
		PlayerHUDWidget->setHP(hitPoints);
	}
}

void ACaptain::stun(float durationInSeconds) {
	if (isStunned) {
		GetWorldTimerManager().ClearTimer(stunTimer);
	}
	isStunned = true;
	GetWorldTimerManager().SetTimer(stunTimer, this, &ACaptain::onStunTimerTimeout, 1, false, durationInSeconds);
	GetAnimInstance()->StopAllAnimationOverrides();
	EnableAttackCollisionBox(false);
}

void ACaptain::onStunTimerTimeout() {
	isStunned = false;
}

void ACaptain::onRestartTimerTimeout() {
	MyGameInstance->restartGame();
}

void ACaptain::collectItem(CollectibleType itemType) {
	UGameplayStatics::PlaySound2D(GetWorld(), CollectItemSound);

	switch (itemType) {
	case CollectibleType::HealthPotion:
		updateHP(hitPoints + 25);
		break;
	case CollectibleType::Diamond:
		MyGameInstance->collectDiamond();
		PlayerHUDWidget->setDiamonds(MyGameInstance->CollectedDiamondCount);
		break;
	case CollectibleType::DoubleJumpUpgrade:
		unlockDoubleJump();
		break;
	}
}

void ACaptain::unlockDoubleJump() {
	JumpMaxCount = 2;
	MyGameInstance->IsDoubleJumpUnlocked = true;
}

void ACaptain::deactivate() {
	if (isActive) {
		isActive = false;
		canAttack = false;
		canMove = false;

		GetCharacterMovement()->StopMovementImmediately();
	}
}

void ACaptain::quitGame() {
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
