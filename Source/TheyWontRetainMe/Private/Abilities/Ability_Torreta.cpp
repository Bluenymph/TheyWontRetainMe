#include "Abilities/Ability_Torreta.h"
#include "LogMacros.h"
#include "Actors/TurretActor.h"
#include "Interfaces/AbilityVisualInterface.h"
#include "Systems/EnemiesManager.h"
#include "Systems/GameManager.h"

void UAbility_Torreta::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	
	if (!InOwner || !VisualActorClass) return;
	
	AbilityOwner = InOwner;

	//Buscamos el suelo
	FVector Start = AbilityOwner->GetActorLocation() + (AbilityOwner->GetActorRightVector() * 150.0f);
	FVector End = Start + (FVector::UpVector * -500.0f);

	FHitResult GroundHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AbilityOwner); // Ignorar al jugador
	FVector FinalSpawnLocation = Start;

	if (GetWorld()->LineTraceSingleByChannel(GroundHit, Start, End, ECC_Visibility, QueryParams))
	{
		FinalSpawnLocation = GroundHit.Location;
		FinalSpawnLocation.Z += ZOffset;
	}
	
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AbilityOwner;
	SpawnParams.Instigator = AbilityOwner->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	VisualActor = GetWorld()->SpawnActor<ATurretActor>(VisualActorClass, FinalSpawnLocation, AbilityOwner->GetActorRotation(), SpawnParams);
	TurretActor = Cast<ATurretActor>(VisualActor);
	if (TurretActor)
	{
		TurretActor->InitDestructionTimer(TiempoVida);
		GetWorld()->GetTimerManager().SetTimer(
		FTimerHandler_TiempoVida,
		this,
		&UAbility_Torreta::AutoDestruction,
		TiempoVida,
		true);
	}
	
	if (VisualActor) VisualActor->Tags.Add(FName("Minion"));
	
	
	if (VisualActor && VisualActor->GetClass()->ImplementsInterface(UAbilityVisualInterface::StaticClass()))
	{
		IAbilityVisualInterface::Execute_SetParentAbility(VisualActor, this);
	}
	
	if (GetWorld() && VisualActor)
	{
		UGameManager* GameManager = VisualActor->GetGameInstance()->GetSubsystem<UGameManager>();
		int32 Current = FCString::Atoi(*GameManager->GameStatistics.TurretsNum);
		Current++;
		GameManager->GameStatistics.TurretsNum = FString::FromInt(Current);
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		FTimerHandler_Shoot,
		this,
		&UAbility_Torreta::TorretaShoot,
		Cadencia,
		true);
}

void UAbility_Torreta::TickAbility(float DeltaTime)
{
	Super::TickAbility(DeltaTime);
	if (TurretActor)
	{
		TurretActor->RotateTurret(EnemiesManager->GetNearestEnemy(TurretActor->GetActorLocation()));
	}
}

void UAbility_Torreta::TorretaShoot()
{
	if (!EnemiesManager) return;
	
	TurretActor->Shoot(DanioBalas, BulletClass);
	
}

void UAbility_Torreta::AutoDestruction()
{
	if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(FTimerHandler_Shoot);
	MarkAsGarbage();
}
