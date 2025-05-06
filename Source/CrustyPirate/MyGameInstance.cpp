// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>

void UMyGameInstance::setPlayerHP(int newHP) {
	this->PlayerHP = newHP;
}

void UMyGameInstance::collectDiamond() {
	CollectedDiamondCount++;
}

void UMyGameInstance::changeLevel(int newLevelIndex) {
	if (newLevelIndex < 0 || newLevelIndex>levels.size() - 1)
		return;
	CurrentLevelIndex = newLevelIndex;
	UGameplayStatics::OpenLevel(GetWorld(), FName(levels[CurrentLevelIndex]));
}

void UMyGameInstance::restartGame() {
	PlayerHP = 100;
	CollectedDiamondCount = 0;
	IsDoubleJumpUnlocked = false;

	changeLevel(0);
}
