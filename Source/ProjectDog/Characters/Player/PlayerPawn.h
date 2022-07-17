// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPawn.generated.h"

class AWeapon;
class UMainGameWidget;

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USoundBase;

UCLASS(config=Game)
class APlayerPawn : public APawn
{
	GENERATED_BODY()

	friend class UMainGameWidget;

protected:
	/** Gameplay members */
	/* This is when calculating the trace to determine what the weapon has hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponRange = 5000.0f;
	
	/* This is multiplied by the direction vector when the weapon trace hits something to apply velocity to the component that is hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDamage = 3.0f;

	/** Health Value. Dead if less or equal to 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 RollCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float RollDiceCooldownSeconds = 5.0f;

	TQueue<int32> RecentDiceResults;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 AmmoCount = 0;

	/** Can be either set in the editor or Blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	AWeapon* PossessedWeapon = nullptr;

	/** UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	UMainGameWidget* GameUI = nullptr;

	/** Cameras */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* CameraComponent;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sounds)
	USoundBase* FireSound;

public:
	APlayerPawn();

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override final;

	void OnEliminateEnemy(int32 ScoreToAdd) { Score += ScoreToAdd; }

	/** Roll a dice to get bullets count */
	void RollDice();

	/** Fires a virtual projectile. */
	void OnFire();

	void JoystickMovedYaw(float Rate);
	void JoystickMovedPitch(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override final;

private:
	FVector GetAimLocation() const;
	void AimWeapon(float Rate = 0.0f);
	bool IsAcceptingJoystickMovement() const;
};

