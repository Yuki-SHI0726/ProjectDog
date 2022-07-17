// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainGameWidget.h"
#include "Characters/Player/PlayerPawn.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCanvasPanelSlot* HealthBar_ImageCanvasPanelSlot = Cast<UCanvasPanelSlot>(HealthBar_Image->Slot);
	const FMargin HealthBar_ImageOriginMargin = HealthBar_ImageCanvasPanelSlot->GetOffsets();
	HealthBar_ImageOrigin.X = HealthBar_ImageOriginMargin.Top;
	HealthBar_ImageOrigin.Y = HealthBar_ImageOriginMargin.Bottom;

	APlayerPawn* Player = Cast<APlayerPawn>(GetOwningPlayerPawn());
	if (Player)
	{
		SetAmmoCount(Player->AmmoCount);
		SetHealth(Player->Health / 100.0f);
	}
}

void UMainGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	FVector2D MousePositionOnViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	UCanvasPanelSlot* CrossHair_ImageCanvasPanelSlot = Cast<UCanvasPanelSlot>(CrossHair_Image->Slot);
	CrossHair_ImageCanvasPanelSlot->SetPosition(MousePositionOnViewport);
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

void UMainGameWidget::SetHealth(float HealthPercent)
{
	UCanvasPanelSlot* HealthBar_ImageCanvasPanelSlot = Cast<UCanvasPanelSlot>(HealthBar_Image->Slot);
	FMargin HealthBar_ImageMargin = HealthBar_ImageCanvasPanelSlot->GetOffsets();

	HealthBar_ImageMargin.Top = HealthBar_ImageOrigin.X * HealthPercent;
	HealthBar_ImageMargin.Bottom = HealthBar_ImageOrigin.Y * HealthPercent;

	HealthBar_ImageCanvasPanelSlot->SetOffsets(HealthBar_ImageMargin);
}

void UMainGameWidget::RollDice(int32 DiceResult)
{
}
