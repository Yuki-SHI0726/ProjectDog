// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FP_FirstPersonCharacter.generated.h"

class UMainGameWidget;

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USoundBase;
class UAnimMontage;

UCLASS(config=Game)
class AFP_FirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFP_FirstPersonCharacter();

protected:
	/** Gameplay members */
	/* This is when calculating the trace to determine what the weapon has hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponRange = 5000.0f;
	
	/* This is multiplied by the direction vector when the weapon trace hits something to apply velocity to the component that is hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float WeaponDamage = 500000.0f;

	/** UI */
	UPROPERTY(BlueprintReadWrite, Category = UI)
	UMainGameWidget* GameUI = nullptr;

	/** Meshes */
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh */
	UPROPERTY(BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Cameras */
	UPROPERTY(BlueprintReadWrite, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseLookUpRate;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sounds)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* FireAnimation;

public:
	/** Roll a dice to get bullets count */
	void RollDice();

	/** Fires a virtual projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/* 
	 * Performs a trace between two points
	 * 
	 * @param	StartTrace	Trace starting point
	 * @param	EndTrac		Trace end point
	 * @returns FHitResult returns a struct containing trace result - who/what the trace hit etc.
	 */
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
};

