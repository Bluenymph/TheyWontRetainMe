#include "Abilities/Ability_Thunder.h"
#include "LogMacros.h"
#include "Characters/EnemyTemplate.h"
#include "Interfaces/AbilityVisualInterface.h"
#include "Interfaces/HiteableInterface.h"
#include "Systems/EnemiesManager.h"
#include "Systems/GameManager.h"

void UAbility_Thunder::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	AbilityOwner = InOwner;
	
	EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	
	if (EnemiesManager) EnemiesManager->OnEnemyDamaged.AddUniqueDynamic(this, &UAbility_Thunder::TryInvokeThunder);
	
}

void UAbility_Thunder::OnVisualOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnVisualOverlap(OverlappedActor, OtherActor);
	
	if (OverlappedActor && OtherActor && OtherActor->Implements<UEnemyInterface>())
	{
		UGameManager* GameManager = OverlappedActor->GetGameInstance()->GetSubsystem<UGameManager>();
		GameManager->ShowImpacNumber(OverlappedActor->GetActorLocation(),OtherActor,Danio);
		IHiteableInterface::Execute_OnHitReceived(OtherActor, Danio, AbilityOwner);
	}
}

void UAbility_Thunder::TryInvokeThunder(AEnemyTemplate* EnemyTemplate, float Damage)
{
	if (!EnemyTemplate) return;
	bool bSuccess = FMath::RandRange(1, 100) <= Probabilidad;
	
	if (bSuccess)
	{
		FVector SpawnLocation = EnemyTemplate->GetActorLocation() + FVector(0, 0, 300);
		VisualActor = GetWorld()->SpawnActor<AActor>(VisualActorClass, SpawnLocation, FRotator::ZeroRotator);
		VisualActor->UpdateOverlaps(true);
	
		if (VisualActor && VisualActor->GetClass()->ImplementsInterface(UAbilityVisualInterface::StaticClass()))
		{
			IAbilityVisualInterface::Execute_SetParentAbility(VisualActor, this);
		}
	}
}
