#include "Abilities/Upgrade_ReloadDodge.h"
#include "Systems/AbilitiesManager.h"

void UUpgrade_ReloadDodge::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	UAbilitiesManager* AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	AbilitiesManager->SetAbilityReloadDodge(true);
}
