// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <vector>

#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	static inline std::vector<FString> levels = {
		FString(TEXT("Level_1")),
		FString(TEXT("Level_2"))
	};
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PlayerHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CoinsCollected = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int DiamondsCollected = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDoubleJumpUnlocked = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int CurrentLevelIndex = 0;

	void setPlayerHP(int newHP);

	void collectCoins(int count);

	void collectDiamonds(int count);

	void changeLevel(int newLevelIndex);

	UFUNCTION(BlueprintCallable)
	void restartGame();
};
