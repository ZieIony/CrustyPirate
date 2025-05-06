// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelExit.h"
#include "Captain.h"
#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ALevelExit::ALevelExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
	DoorFlipbook->SetupAttachment(RootComponent);
	DoorFlipbook->SetPlayRate(0.0f);
	DoorFlipbook->SetLooping(false);
}

// Called when the game starts or when spawned
void ALevelExit::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OverlapBegin);

	DoorFlipbook->SetPlaybackPosition(0.0f, false);
}

// Called every frame
void ALevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelExit::OverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResults) {
	ACaptain* player = Cast<ACaptain>(otherActor);
	if (player && player->isAlive) {
		if (IsActive) {
			player->deactivate();
			IsActive = false;
			DoorFlipbook->SetPlayRate(1.0f);
			DoorFlipbook->PlayFromStart();
			UGameplayStatics::PlaySound2D(GetWorld(), PlayerEnterSound);
			GetWorldTimerManager().SetTimer(delayTimer, this, &ALevelExit::onDelayTimerTimeout, 1.0f, false, delayTimeSec);
		}
	}
}

void ALevelExit::onDelayTimerTimeout() {
	auto gameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (gameInstance) {
		gameInstance->changeLevel(LevelIndex);
	}

}

