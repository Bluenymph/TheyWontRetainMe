#include "Systems/EnemiesManager.h"
#include "LogMacros.h"
#include "Actors/ObstacleActor.h"
#include "Characters/EnemyTemplate.h"
#include "Components/CharacterAttributes.h"
#include "Interfaces/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void UEnemiesManager::PrewarmEnemyPool(TSubclassOf<AEnemyTemplate> EnemyClass, int32 Amount)
{
	if (!EnemyClass) return;

	FEnemyPool& Pool = EnemyPoolMap.FindOrAdd(EnemyClass);

	for (int32 i = 0; i < Amount; ++i)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
		AEnemyTemplate* NewBullet = GetWorld()->SpawnActor<AEnemyTemplate>(EnemyClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
        
		if (NewBullet)
		{
			//La apagamos inmediatamente y la guardamos para usarla despues
			NewBullet->SetActorHiddenInGame(true);
			NewBullet->SetActorEnableCollision(false);
			Pool.InactiveEnemies.Add(NewBullet);
		}
	}
}

AEnemyTemplate* UEnemiesManager::GetEnemyFromPool(TSubclassOf<AEnemyTemplate> EnemyClass, FVector Location,
	FRotator Rotation)
{
	if (!EnemyClass) return nullptr;

	FEnemyPool& Pool = EnemyPoolMap.FindOrAdd(EnemyClass);
	AEnemyTemplate* EnemyToUse = nullptr;

	if (Pool.InactiveEnemies.Num() > 0)
	{
		EnemyToUse = Pool.InactiveEnemies.Pop();
	}
	else
	{
		//Si no hay spawneamos una nueva, esto no deberia pasar, idealmente...
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		EnemyToUse = GetWorld()->SpawnActor<AEnemyTemplate>(EnemyClass, Location, Rotation, SpawnParams);
	}

	if (EnemyToUse)
	{
		//La movemos a su sitio y la activamos mediante la Interfaz
		EnemyToUse->SetActorLocationAndRotation(Location, Rotation);
		EnemyToUse->GetCharacterAttributes()->SetVidaActual(EnemyToUse->GetCharacterAttributes()->GetVidaMaxima());
		
		//Llamamos a la función de la interfaz para activarla
		IEnemyInterface::Execute_OnActivateEnemy(EnemyToUse,Location,Rotation);
		ActiveEnemies.Add(EnemyToUse);
	}
	
	return EnemyToUse;
}

void UEnemiesManager::ReturnEnemyToPool(AEnemyTemplate* Enemy)
{
	if (!Enemy) return;

	FEnemyPool& Pool = EnemyPoolMap.FindOrAdd(Enemy->GetClass());
	Pool.InactiveEnemies.Add(Enemy);
	ActiveEnemies.Remove(Enemy);
}

void UEnemiesManager::Deinitialize()
{
	StopSpawnEnemiesLoop();
	Super::Deinitialize();
}

#pragma region SpawnEnemiesLoop

void UEnemiesManager::BeginManageEnemiesLoop()
{
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(
			LoopEnemySpawn, 
			this, 
			&UEnemiesManager::SpawnEnemiesLoop, 
			1.0f,
			true
		);

		World->GetTimerManager().SetTimer(
			TimerHandle_ManageEnemies,
			this,
			&UEnemiesManager::ManageEnemies,
			0.02f,
			true
			);
	}
}

void UEnemiesManager::ManageEnemies()
{
	if (!PlayerPawn) return;

	float DeltaTime = GetWorld()->GetDeltaSeconds(); 
	
	for (int32 i = ActiveEnemies.Num() - 1; i >= 0; --i)
	{
		AEnemyTemplate* Enemy = ActiveEnemies[i];
		
		Enemy->UpdateMovement(PlayerPawn, DeltaTime);
	}
}

AEnemyTemplate* UEnemiesManager::GetEnemyUnderTarget()
{
	if (!PlayerPawn) return nullptr;
	
	AEnemyTemplate* BestTarget = nullptr;
	float BestScore = -1.0f; // El Dot Product va de -1 a 1
	const float Accuracy = 0.97f;

	for (int32 i = ActiveEnemies.Num() - 1; i >= 0; --i)
	{
		AEnemyTemplate* Enemy = ActiveEnemies[i];
		FVector DirToEnemy = (Enemy->GetActorLocation() - PlayerPawn->GetActorLocation()).GetSafeNormal();
		float Distance = FVector::Dist(Enemy->GetActorLocation(), PlayerPawn->GetActorLocation());
		float CurrentDot = FVector::DotProduct(PlayerPawn->GetActorForwardVector(), DirToEnemy);

		if (CurrentDot > Accuracy)
		{
			float CurrentScore = CurrentDot / (Distance * 0.001f);
			
			if (CurrentScore > BestScore)
			{
				BestScore = CurrentScore;
				BestTarget = Enemy;
			}
		}
	}
	return BestTarget;
}


void UEnemiesManager::SpawnEnemiesLoop()
{
	if (!PlayerPawn) return;

	FVector SpawnPos;
	FRotator SpawnRot;

	if (GetValidSpawnLocation(SpawnPos, SpawnRot))
	{
		int32 RandomIndex = FMath::RandRange(0, EnemiesToSpawn.Num() - 1);
		GetEnemyFromPool(EnemiesToSpawn[RandomIndex], SpawnPos, SpawnRot);
	}
	ManageEnemies();
	DEBUG(1,FColor::Blue, "%i", ActiveEnemies.Num());
}

void UEnemiesManager::StopSpawnEnemiesLoop()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(LoopEnemySpawn);
	}
}

bool UEnemiesManager::GetValidSpawnLocation(FVector& OutLocation, FRotator& OutRotation)
{
    if (!PlayerPawn) return false;

    //Cálculo de coordenadas iniciales
    float Angle = FMath::RandRange(0.f, 2.f * PI);
    float Radius = FMath::RandRange(1200.0f, 2000.0f); 
    FVector PlayerLoc = PlayerPawn->GetActorLocation();
    
    FVector StartTrace(PlayerLoc.X + (FMath::Cos(Angle) * Radius), PlayerLoc.Y + (FMath::Sin(Angle) * Radius), PlayerLoc.Z + 1000.f);
    FVector EndTrace(StartTrace.X, StartTrace.Y, PlayerLoc.Z - 1000.f);
    
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerPawn);

    FCollisionShape SweepSphere = FCollisionShape::MakeSphere(22.f); 

    //Primer Test: Sweep (Encuentra el suelo y evita paredes laterales)
    if (GetWorld()->SweepSingleByChannel(HitResult, StartTrace, EndTrace, FQuat::Identity, ECC_Visibility, SweepSphere, QueryParams))
    {
        if (HitResult.bStartPenetrating) return false;

        //Filtro de Actores (Interfaz/Obstáculos)
        if (AActor* HitActor = HitResult.GetActor())
        {
            if (HitActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()) || 
                HitActor->IsA(AObstacleActor::StaticClass()))
            {
                return false;
            }
        }

        //Test de Precipicios (Solo si lo anterior es válido)
        const float CheckDist = 22.f;
        const FVector Center = HitResult.Location;
        const FVector CheckOffsets[4] = { 
            FVector(CheckDist, 0, 0), FVector(-CheckDist, 0, 0), 
            FVector(0, CheckDist, 0), FVector(0, -CheckDist, 0) 
        };

        for (int32 i = 0; i < 4; ++i)
        {
            FHitResult EdgeHit;
            FVector TestPoint = Center + CheckOffsets[i];
            // Rayo corto hacia abajo: si no hay nada en 150 unidades, hay vacío
            if (!GetWorld()->LineTraceSingleByChannel(EdgeHit, TestPoint, TestPoint - FVector(0, 0, 150.f), ECC_Visibility, QueryParams))
            {
                return false; 
            }
        }

        //Configuración final
        OutLocation = HitResult.Location; 
        OutLocation.Z += 80.0f; 

        OutRotation = UKismetMathLibrary::FindLookAtRotation(OutLocation, PlayerLoc);
        OutRotation.Pitch = 0.f;
        OutRotation.Roll = 0.f;
        
        return true;
    }

    return false;
}
#pragma endregion
