// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerPawn.h"
#include "UI/MainGameWidget.h"
#include "Weapons/Weapon.h"

#include "Editor.h"
#include "GameFramework/DamageType.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

#define COLLISION_WEAPON		ECC_GameTraceChannel1

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

APlayerPawn::APlayerPawn()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
}

float APlayerPawn::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	GameUI->SetHealth(Health / 100.0f);

	if (Health <= 0.0f)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Level_Menu");
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APlayerPawn::OnEliminateEnemy(int32 ScoreToAdd)
{
	Score += ScoreToAdd;
	GameUI->SetScore(Score);
}

void APlayerPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("RollDice", IE_Pressed, this, &APlayerPawn::RollDice);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPawn::OnFire);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerPawn::AimWeapon);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerPawn::JoystickMovedYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerPawn::AimWeapon);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerPawn::JoystickMovedPitch);
}

FVector APlayerPawn::GetAimLocation() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FVector Origin;
	FVector Direction;

	PlayerController->DeprojectMousePositionToWorld(Origin, Direction);
	const FVector AimLocation = Origin + Direction * PlayerController->HitResultTraceDistance;
	return AimLocation;
}

void APlayerPawn::AimWeapon(float Rate /* = 0.0f*/)
{
	const FRotator ShootRotation = UKismetMathLibrary::FindLookAtRotation(PossessedWeapon->GetActorLocation(), GetAimLocation());
	PossessedWeapon->SetActorRotation(ShootRotation);
}

bool APlayerPawn::IsAcceptingJoystickMovement() const
{
	// Focus in game
#if UE_EDITOR
	if (UEditorEngine* Editor = CastChecked<UEditorEngine>(GEngine))
	{
		if (Editor->bIsSimulatingInEditor)
		{
			return false;
		}
	}

	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		UGameViewportClient* GameViewportClient = GetWorld()->GetGameViewport();
		return GameViewportClient->IsFocused(GEngine->GameViewport->Viewport);	
	}
#endif //UE_EDITOR

	return false;
}

void APlayerPawn::RollDice()
{
	// Cooldown
	static bool bInCooldown = false;
	if (bInCooldown)
	{
		return;
	}

	--RollCount;
	FTimerHandle Handle;
	bInCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(Handle, []()
		{
			bInCooldown = false;
		}, RollDiceCooldownSeconds, false);

	// Roll a dice to Add
	const int32 DiceResult = FMath::RandRange(1, 6);
	AmmoCount += DiceResult;

	// UI
	GameUI->RollDice(DiceResult);
	GameUI->SetAmmoCount(AmmoCount);
}

void APlayerPawn::OnFire()
{
	// Exit if no bullets or no possessed weapon
	if (AmmoCount <= 0 || 
		!PossessedWeapon)
	{
		return;
	}

	// Reduce bullets and notify UI
	--AmmoCount;
	GameUI->SetAmmoCount(AmmoCount);

	// Play a sound if there is one
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult HitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes;
	HitObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	if (PlayerController->GetHitResultUnderCursorForObjects(HitObjectTypes, true, HitResult))
	{
		// Deal with impact
		AActor* DamagedActor = HitResult.GetActor();
		UPrimitiveComponent* DamagedComponent = HitResult.GetComponent();

		// If we hit an actor, apply damage. If with a component that is simulating physics, apply an impulse
		if ((DamagedActor != nullptr) &&
			(DamagedActor != this) &&
			(DamagedComponent != nullptr))
		{
			UGameplayStatics::ApplyDamage(DamagedActor, WeaponDamage, GetController(), this, UDamageType::StaticClass());

			if (DamagedComponent->IsSimulatingPhysics())
			{
				DamagedComponent->AddImpulseAtLocation(PossessedWeapon->GetActorForwardVector() * WeaponDamage, HitResult.Location);
			}
		}
	}
}

void APlayerPawn::JoystickMovedYaw(float Rate)
{
	if (IsAcceptingJoystickMovement())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		float MouseX;
		float MouseY;
		PlayerController->GetMousePosition(MouseX, MouseY);

		MouseX += Rate * 5.0f;
		PlayerController->SetMouseLocation(MouseX, MouseY);
	}

	AimWeapon();
}

void APlayerPawn::JoystickMovedPitch(float Rate)
{
	if (IsAcceptingJoystickMovement())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		float MouseX;
		float MouseY;
		PlayerController->GetMousePosition(MouseX, MouseY);

		MouseY += Rate * 5.0f;
		PlayerController->SetMouseLocation(MouseX, MouseY);
	}

	AimWeapon();
}