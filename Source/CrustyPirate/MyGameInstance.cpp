// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include <Kismet/GameplayStatics.h>

void UMyGameInstance::fireOnPlayerInfoChanged() {
	OnPlayerInfoChangedEvent.Broadcast(PlayerHP, CoinsCollected, DiamondsCollected, KeysOwned, SwordsOwned);
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

void UMyGameInstance::collectKey() {
	KeysOwned++;
	fireOnPlayerInfoChanged();
}

void UMyGameInstance::useKey() {
	KeysOwned--;
	fireOnPlayerInfoChanged();
}

void UMyGameInstance::collectSwords(int count) {
	SwordsOwned += count;
	fireOnPlayerInfoChanged();
}

void UMyGameInstance::useSword() {
	SwordsOwned--;
	fireOnPlayerInfoChanged();
}

void UMyGameInstance::finishLevel() {
	OnLevelFinishedEvent.Broadcast();
}

void UMyGameInstance::nextLevel() {
	CurrentLevelIndex++;
	KeysOwned = 0;
	SwordsOwned = 0;
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
	KeysOwned = 0;
	SwordsOwned = 0;

	fireOnPlayerInfoChanged();

	CurrentLevelIndex = 0;
	UGameplayStatics::OpenLevel(GetWorld(), FName(levels[CurrentLevelIndex]));
}
