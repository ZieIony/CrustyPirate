// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>

#include "Destructible.generated.h"

class UPaperFlipbookComponent;
class UPaperZDAnimationComponent;

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

	// Sets default values for this actor's properties
	ADestructible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void takeDamage(int damageAmount);
};
