// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameWidget.generated.h"

class AFP_FirstPersonCharacter;

class UTextBlock;
class UImage;

/**
 * Main game UI widget
 */
UCLASS()
class PROJECTDOG_API UMainGameWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* AmmoCount_TextBlock = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UImage* Ammo_Image = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UImage* HealthBar_Image = nullptr;
	
public:
	virtual void NativeConstruct() override final;

	void SetAmmoCount(int32 AmmoCount);
	void SetHealth(float Health);
	void RollDice(int32 DiceResult);
};
