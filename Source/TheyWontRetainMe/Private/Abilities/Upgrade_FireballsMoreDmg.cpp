#include "Abilities/Upgrade_FireballsMoreDmg.h"
#include "Abilities/Ability_Fireballs.h"
#include "Systems/AbilitiesManager.h"

void UUpgrade_FireballsMoreDmg::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	UAbilitiesManager* AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	
	int32 Position = AbilitiesManager->FindAbilityByName(FText::FromString("Bolas de Fuego"));
	if (Position > -1)
	{
		UAbility_Fireballs* Fireballs = Cast<UAbility_Fireballs>(AbilitiesManager->ActiveAbilities[Position]);
		if (Fireballs) Fireballs->SetDamage(Fireballs->GetDamage() + CantidadAumentar);
	}
}
