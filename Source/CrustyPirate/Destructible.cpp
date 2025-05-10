// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible.h"
#include "PaperZDAnimationComponent.h"
#include <PaperCharacter.h>
#include "PaperFlipbookComponent.h"
#include "Captain.h"
#include "CollectibleItem.h"

// Sets default values
ADestructible::ADestructible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);

	AnimationComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("Animation"));
	AnimationComponent->InitRenderComponent(Sprite);
}

// Called when the game starts or when spawned
void ADestructible::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADestructible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructible::takeDamage(int damageAmount) {
	if (CurrentHitPoints <= 0)
		return;

	CurrentHitPoints = std::max(0, CurrentHitPoints - damageAmount);

	if (CurrentHitPoints == 0) {
		destroyAndSpawnContents();
	} else {
		AnimationComponent->GetAnimInstance()->JumpToNode(FName("jumpHit"));
	}
}

void ADestructible::destroyAndSpawnContents() {
	if (ContentsClass) {
		for (int i = 0; i < ContentsCount; i++) {
			auto* collectibleItem = GetWorld()->SpawnActor<ACollectibleItem>(
				ContentsClass,
				GetActorLocation(),
				FRotator::ZeroRotator
			);
			auto force = FVector((float)rand() / RAND_MAX * 2 - 1, 0, 1);
			force.Normalize();
			collectibleItem->CapsuleComp->AddImpulse(force * LaunchContentsForce);
		}
	}
	if (ContentsClass2) {
		for (int i = 0; i < ContentsCount2; i++) {
			auto* collectibleItem = GetWorld()->SpawnActor<ACollectibleItem>(
				ContentsClass2,
				GetActorLocation(),
				FRotator::ZeroRotator
			);
			auto force = FVector((float)rand() / RAND_MAX * 2 - 1, 0, 1);
			force.Normalize();
			collectibleItem->CapsuleComp->AddImpulse(force * LaunchContentsForce);
		}
	}
	Destroy();
}

