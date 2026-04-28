#include "Abilities/Ability_Torreta.h"

#include "LogMacros.h"
#include "Actors/TurretActor.h"
#include "Interfaces/AbilityVisualInterface.h"
#include "Systems/EnemiesManager.h"

void UAbility_Torreta::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	
	if (!InOwner || !VisualActorClass) return;
	
	AbilityOwner = InOwner;

	FVector SpawnLocation = AbilityOwner->GetActorLocation();
	VisualActor = GetWorld()->SpawnActor<ATurretActor>(VisualActorClass, SpawnLocation, FRotator::ZeroRotator);
	TurretActor = Cast<ATurretActor>(VisualActor);
	
	if (VisualActor && VisualActor->GetClass()->ImplementsInterface(UAbilityVisualInterface::StaticClass()))
	{
		IAbilityVisualInterface::Execute_SetParentAbility(VisualActor, this);
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
	LOG("PUMM")
	
}
