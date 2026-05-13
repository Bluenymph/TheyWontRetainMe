#include "Abilities/Heal.h"
#include "Characters/PlayerTemplate.h"
#include "Components/CharacterAttributes.h"

void UHeal::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate && PlayerTemplate->GetCharacterAttributes())
	{
		PlayerTemplate->GetCharacterAttributes()->Curar(CantidadCura);
	}
}
