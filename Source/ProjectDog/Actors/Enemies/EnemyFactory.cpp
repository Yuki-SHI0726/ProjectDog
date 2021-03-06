// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemies/EnemyFactory.h"
#include "Actors/Enemies/EnemyProjectile.h"
#include "Actors/Triggers/ProjectileHandlerTriggerBox.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"

AEnemyFactory::AEnemyFactory()
{
	FindAllEnemyProjectileBlueprints();
}

void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();

	ProjectileHandlerTriggerBox = Cast<AProjectileHandlerTriggerBox>(UGameplayStatics::GetActorOfClass(GetWorld(), AProjectileHandlerTriggerBox::StaticClass()));

	/** Spawn enemy projectiles timer */
	FTimerHandle SpawnEnemyHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "SpawnEnemy");
	GetWorld()->GetTimerManager().SetTimer(SpawnEnemyHandle, TimerDelegate, SpawnCooldownSeconds, true);
}

void AEnemyFactory::SpawnEnemy()
{
	// Look at random location towards to the trigger box
	UBoxComponent* BoxComponent = Cast<UBoxComponent>(ProjectileHandlerTriggerBox->GetCollisionComponent());
	if (BoxComponent)
	{
		const FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(ProjectileHandlerTriggerBox->GetActorLocation(), BoxComponent->GetScaledBoxExtent());
		const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), RandomPoint);
		SetActorRotation(NewRotation);
	}

	// Spawn a random projectile at factory's rotation location
	const int32 RandomIndex = FMath::RandRange(0, Projectiles.Num() - 1);
	UClass* RandomProjectile = Projectiles[RandomIndex];
	GetWorld()->SpawnActor<AEnemyProjectile>(RandomProjectile, GetActorLocation(), GetActorRotation());
}

void AEnemyFactory::FindAllEnemyProjectileBlueprints()
{
	// Load the derived NPC blueprints from memory
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FString> ContentPaths;
	ContentPaths.Add(TEXT("/Game/ProjectDog/Actors/Enemies"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	TSet<FName> DerivedNames;
	TArray<FName> BaseNames;
	TSet<FName> Excluded;
	BaseNames.Add(AEnemyProjectile::StaticClass()->GetFName());
	AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);

	// Get all blueprint classes
	FARFilter Filter;
	Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add(TEXT("/Game/ProjectDog/Actors/Enemies"));
	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	for (const FAssetData& Asset : AssetList)
	{
		// Get the the class this blueprint generates (this is stored as a full path)
		if (Asset.TagsAndValues.Contains(TEXT("GeneratedClass")))
		{
			// Convert path to just the name part
			const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(Asset.GetTagValueRef<FString>(TEXT("GeneratedClass")));
			const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);

			// Check if this class is in the derived set
			if (!DerivedNames.Contains(*ClassName))
			{
				continue;
			}

			// set default pawn class to our Blueprinted character
			ConstructorHelpers::FClassFinder<AEnemyProjectile> EnemyProjectileFinder(*ClassObjectPath);
			if (EnemyProjectileFinder.Class != nullptr)
			{
				Projectiles.Emplace(EnemyProjectileFinder.Class);
			}
		}
	}
}
