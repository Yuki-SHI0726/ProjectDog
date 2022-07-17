// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyProjectile.h"
#include "Characters/Player/FP_FirstPersonCharacter.h"
#include "Actors/Triggers/ProjectileHandlerTriggerBox.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyProjectile::AEnemyProjectile()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnBeginOverlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = StaticMeshComponent;
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 800.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;		// Not applying gravity at all since we are in space
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 60.0f;
}

float AEnemyProjectile::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;

	// If Dead
	if (Health <= 0.0f)
	{
		// Player gain score
		if (AFP_FirstPersonCharacter* Player = Cast<AFP_FirstPersonCharacter>(DamageCauser))
		{
			Player->OnEliminateEnemy(Score);
		}
		GetWorld()->DestroyActor(this);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AProjectileHandlerTriggerBox* ProjectileHandlerTriggerBox = Cast<AProjectileHandlerTriggerBox>(OtherActor))
	{
		AFP_FirstPersonCharacter* const Player = GetWorld()->GetFirstPlayerController()->GetPawn<AFP_FirstPersonCharacter>();
		UGameplayStatics::ApplyDamage(Player, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

		GetWorld()->DestroyActor(this);
	}
}
