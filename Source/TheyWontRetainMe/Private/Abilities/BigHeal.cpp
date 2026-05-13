#include "Abilities/BigHeal.h"
#include "Characters/PlayerTemplate.h"
#include "Components/CharacterAttributes.h"

void UBigHeal::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate)
	{
		PlayerTemplate->GetCharacterAttributes()->SetVidaActual(
			PlayerTemplate->GetCharacterAttributes()->GetVidaMaxima());
	}
}
