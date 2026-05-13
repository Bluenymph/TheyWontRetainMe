#include "Abilities/Nerf_SlowEnemies.h"

#include "Systems/AbilitiesManager.h"

void UNerf_SlowEnemies::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	UAbilitiesManager* AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	AbilitiesManager->SetSlowEnemies(true);
}
