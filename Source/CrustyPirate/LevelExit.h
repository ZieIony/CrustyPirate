// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <PaperFlipbookComponent.h>
#include <Sound/SoundBase.h>
#include <Engine/TimerHandle.h>

#include "LevelExit.generated.h"

UCLASS()
class CRUSTYPIRATE_API ALevelExit : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* DoorFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* PlayerEnterSound;

	FTimerHandle delayTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float delayTimeSec = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsActive = true;

	// Sets default values for this actor's properties
	ALevelExit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapBegin(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComponent,
		int32 otherBodyIndex,
		bool fromSweep,
		const FHitResult& sweepResults
	);

	void onDelayTimerTimeout();
};
