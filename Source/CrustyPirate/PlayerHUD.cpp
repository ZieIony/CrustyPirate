// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::setHP(int newHP) {
	FString text = FString::Printf(TEXT("HP: %d"), newHP);
	HPText->SetText(FText::FromString(text));
}

void UPlayerHUD::setCoins(int count) {
	FString text = FString::Printf(TEXT("coins: %d"), count);
	CoinsText->SetText(FText::FromString(text));
}

void UPlayerHUD::setDiamonds(int count) {
	FString text = FString::Printf(TEXT("diamonds: %d"), count);
	DiamondsText->SetText(FText::FromString(text));
}

void UPlayerHUD::setLevel(int index) {
	FString text = FString::Printf(TEXT("level: %d"), index);
	LevelText->SetText(FText::FromString(text));
}
