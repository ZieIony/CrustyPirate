// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "EnemyHealthBar.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDCharacter.h"
#include <AnimSequences/PaperZDAnimSequence.h>
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/TimerHandle.h>

AEnemy::AEnemy() {
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectorSphere"));
	PlayerDetectorSphere->SetupAttachment(RootComponent);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	AttackCollisionBox->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay() {
	Super::BeginPlay();

	PlayerDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectorOverlapBegin);
	PlayerDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectorOverlapEnd);

	updateHP(hitPoints);

	OnAttackOverrideEndDelegate.BindUObject(this, &AEnemy::onAttackOverrideAnimEnd);
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackBoxOverlapBegin);
	EnableAttackCollisionBox(false);
}

void AEnemy::Tick(float dt) {
	Super::Tick(dt);

	if (getIsAlive() && FollowTarget && !isStunned) {
		float distX = FollowTarget->GetActorLocation().X - GetActorLocation().X;
		if (fabs(distX) > stopDistanceToTarget && canMove) {
			float dir = distX > 0 ? 1.0f : -1.0f;
			AddMovementInput({ 1,0,0 }, dir);
			updateDirection(dir);
		} else {
			if (FollowTarget->getIsAlive()) {
				attack();
			}
		}
	}
}

void AEnemy::DetectorOverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResults) {
	auto captain = Cast<ACaptain>(otherActor);
	if (captain) {
		FollowTarget = captain;
	}
}

void AEnemy::DetectorOverlapEnd(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex) {
	FollowTarget = nullptr;
}

bool AEnemy::getIsAlive() {
	return hitPoints > 0;
}

void AEnemy::updateDirection(float moveDirection) {
	FRotator currentRotation = GetActorRotation();
	if (moveDirection < 0) {
		SetActorRotation({ currentRotation.Pitch,180,currentRotation.Roll });
	} else if (moveDirection > 0) {
		SetActorRotation({ currentRotation.Pitch,0,currentRotation.Roll });
	}
}

void AEnemy::updateHP(int newHP) {
	hitPoints = newHP;
	OnHealthChangedEvent.Broadcast(newHP);
}

void AEnemy::takeDamage(int damageAmount, float stunDuration, float stunForce) {
	if (!getIsAlive())
		return;

	updateHP(std::max(0, hitPoints - damageAmount));

	if (hitPoints == 0) {
		canMove = false;
		canAttack = false;

		GetAnimInstance()->JumpToNode(FName("jumpDie"));
		EnableAttackCollisionBox(false);
	} else {
		GetAnimInstance()->JumpToNode(FName("jumpTakeHit"));
		stun(stunDuration);
		if (FollowTarget) {
			auto direction = GetActorLocation().X - FollowTarget->GetActorLocation().X;
			auto stunImpulse = FVector(direction, 0, abs(direction));
			stunImpulse.Normalize();
			stunImpulse = stunImpulse * stunForce;
			GetCharacterMovement()->AddImpulse(stunImpulse);
		}
	}
}

void AEnemy::stun(float durationInSeconds) {
	if (isStunned) {
		GetWorldTimerManager().ClearTimer(stunTimer);
	}
	isStunned = true;
	GetWorldTimerManager().SetTimer(stunTimer, this, &AEnemy::onStunTimerTimeout, 1, false, durationInSeconds);
	GetAnimInstance()->StopAllAnimationOverrides();
	EnableAttackCollisionBox(false);
}

void AEnemy::onStunTimerTimeout() {
	isStunned = false;
}

void AEnemy::attack() {
	if (getIsAlive() && canAttack && !isStunned) {
		canAttack = false;
		canMove = false;

		GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("DefaultSlot"), 1, 0, OnAttackOverrideEndDelegate);

		GetWorldTimerManager().SetTimer(attackCooldownTimer, this, &AEnemy::onAttackCooldownTimerTimeout, 1, false, attackCooldownInSeconds);
	}
}

void AEnemy::onAttackCooldownTimerTimeout() {
	if (getIsAlive()) {
		canAttack = true;
	}
}

void AEnemy::onAttackOverrideAnimEnd(bool completed) {
	if (getIsAlive()) {
		canMove = true;
	}
}

void AEnemy::AttackBoxOverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResults) {
	auto player = Cast<ACaptain>(otherActor);
	if (player) {
		player->takeDamage(attackDamage, attackStunDuration);
	}
}

void AEnemy::EnableAttackCollisionBox(bool enable) {
	if (enable) {
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	} else {
		AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}

}
