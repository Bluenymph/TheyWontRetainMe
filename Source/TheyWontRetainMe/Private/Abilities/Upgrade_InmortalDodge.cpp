#include "Abilities/Upgrade_InmortalDodge.h"

#include "Systems/AbilitiesManager.h"

void UUpgrade_InmortalDodge::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	UAbilitiesManager* AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	AbilitiesManager->SetAbility_InmortalDodge(true);
}
