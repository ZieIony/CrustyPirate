// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>

void UMyGameInstance::setPlayerHP(int newHP) {
	this->PlayerHP = newHP;
}

void UMyGameInstance::collectCoins(int count) {
	CoinsCollected += count;
}

void UMyGameInstance::collectDiamonds(int count) {
	DiamondsCollected += count;
}

void UMyGameInstance::changeLevel(int newLevelIndex) {
	if (newLevelIndex < 0 || newLevelIndex>levels.size() - 1)
		return;
	CurrentLevelIndex = newLevelIndex;
	UGameplayStatics::OpenLevel(GetWorld(), FName(levels[CurrentLevelIndex]));
}

void UMyGameInstance::restartGame() {
	PlayerHP = 100;
	DiamondsCollected = 0;
	IsDoubleJumpUnlocked = false;

	changeLevel(0);
}
