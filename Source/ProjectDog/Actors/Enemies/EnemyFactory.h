// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.generated.h"

class AEnemyProjectile;

/**
 * Spawns different enemy projectiles from time to time randomly at this factory actors's location and rotation
 */
UCLASS()
class PROJECTDOG_API AEnemyFactory : public AActor
{
	GENERATED_BODY()
	
private:
	/** Cooldown time between spawning enemies in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = true))
	float SpawnCooldownSeconds = 5.0f;

	/** Stores enemy projectiles for spawning. Initialized during BeginPlay() */
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = true))
	TArray<UClass*> Projectiles;

	FTimerHandle SpawnEnemyHandle;
	FTimerDelegate TimerDelegate;

public:	
	AEnemyFactory();

	virtual void BeginPlay() override final;

	UFUNCTION()
	void SpawnEnemy();
};
