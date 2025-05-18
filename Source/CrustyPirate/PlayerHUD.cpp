// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::setHealth(int newHP) {
	FString text = FString::Printf(TEXT("HP: %d"), newHP);
	HealthText->SetText(FText::FromString(text));
}

void UPlayerHUD::setCoins(int count) {
	FString text = FString::Printf(TEXT("coins: %d"), count);
	CoinsText->SetText(FText::FromString(text));
}

void UPlayerHUD::setDiamonds(int count) {
	FString text = FString::Printf(TEXT("diamonds: %d"), count);
	DiamondsText->SetText(FText::FromString(text));
}
