// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>

void UMyGameInstance::fireOnPlayerInfoChanged() {
	OnPlayerInfoChangedEvent.Broadcast(PlayerHP, CoinsCollected, DiamondsCollected);
}

void UMyGameInstance::setPlayerHP(int newHP) {
	PlayerHP = newHP;
	fireOnPlayerInfoChanged();
}

void UMyGameInstance::collectCoins(int count) {
	CoinsCollected += count;
	fireOnPlayerInfoChanged();
}

void UMyGameInstance::collectDiamonds(int count) {
	DiamondsCollected += count;
	fireOnPlayerInfoChanged();
}

void UMyGameInstance::changeLevel(int newLevelIndex) {
	if (newLevelIndex < 0 || newLevelIndex>levels.size() - 1)
		return;
	CurrentLevelIndex = newLevelIndex;
	UGameplayStatics::OpenLevel(GetWorld(), FName(levels[CurrentLevelIndex]));
}

void UMyGameInstance::collectMap() {
	MapsCollected++;
}

void UMyGameInstance::restartGame() {
	PlayerHP = 100;
	CoinsCollected = 0;
	DiamondsCollected = 0;
	MapsCollected = 0;
	IsDoubleJumpUnlocked = false;

	fireOnPlayerInfoChanged();

	changeLevel(0);
}
