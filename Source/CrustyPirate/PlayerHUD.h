// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>

#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CoinsText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DiamondsText;

	UFUNCTION(BlueprintCallable)
	void setHealth(int newHP);

	UFUNCTION(BlueprintCallable)
	void setCoins(int count);

	UFUNCTION(BlueprintCallable)
	void setDiamonds(int count);
};
