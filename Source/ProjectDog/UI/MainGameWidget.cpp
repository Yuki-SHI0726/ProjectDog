// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainGameWidget.h"
#include "Characters/Player/FP_FirstPersonCharacter.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AFP_FirstPersonCharacter* Player = Cast<AFP_FirstPersonCharacter>(GetOwningPlayerPawn());
	AmmoCount_TextBlock->SetText(FText::AsNumber(Player->AmmoCount));
}

void UMainGameWidget::SetAmmoCount(int32 AmmoCount)
{
	AmmoCount_TextBlock->SetText(FText::AsNumber(AmmoCount));

	if (AmmoCount > 0)
	{
		Ammo_Image->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Ammo_Image->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainGameWidget::RollDice(int32 DiceResult)
{
}
