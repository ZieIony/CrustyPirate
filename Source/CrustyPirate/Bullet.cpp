// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <PaperFlipbookComponent.h>
#include <Components/CapsuleComponent.h>
#include "Captain.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + Velocity * DeltaTime);
}

void ABullet::OverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResults) {
	ACaptain* player = Cast<ACaptain>(otherActor);
	if (player){
		player->takeDamage(attackDamage, attackStunDuration, attackStunForce, this);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
	GetWorld()->SpawnActor<AParticle>(ParticleClass, GetActorLocation(), FRotator::ZeroRotator);
	Destroy();
}

