#include "Abilities/Ability_Dodge.h"
#include "Systems/AbilitiesManager.h"

void UAbility_Dodge::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	UAbilitiesManager* AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	AbilitiesManager->SetDodgeUnlocked(true);
}
