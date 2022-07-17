// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameWidget.generated.h"

class APlayerPawn;

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
	/** CrossHair indicating shooting direction, always follows the mouse cursor */
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UImage* CrossHair_Image = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* AmmoCount_TextBlock = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UImage* Ammo_Image = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UImage* HealthBar_Image = nullptr;

	FVector2D HealthBar_ImageOrigin;
	
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* Score_TextBlock = nullptr;

public:
	virtual void NativeConstruct() override final;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override final;

	void SetAmmoCount(int32 AmmoCount);
	void SetHealth(float HealthPercent);
	void SetScore(int32 Score);
	void RollDice(int32 DiceResult);
};
