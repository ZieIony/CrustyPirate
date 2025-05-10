// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "Destructible.generated.h"

class UPaperFlipbookComponent;
class UPaperZDAnimationComponent;
class ACollectibleItem;

UCLASS()
class CRUSTYPIRATE_API ADestructible : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbookComponent> Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperZDAnimationComponent> AnimationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TotalHitPoints = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHitPoints = TotalHitPoints;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACollectibleItem> ContentsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ContentsCount = 3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACollectibleItem> ContentsClass2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ContentsCount2 = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchContentsForce = 1;

	// Sets default values for this actor's properties
	ADestructible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void takeDamage(int damageAmount);

	void destroyAndSpawnContents();
};
