#include "Abilities/Buff_MoreAmmo.h"
#include "Characters/PlayerTemplate.h"
#include "Components/CharacterAttributes.h"

void UBuff_MoreAmmo::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate && PlayerTemplate->GetCharacterAttributes())
	{
		PlayerTemplate->GetCharacterAttributes()->SetMaxBalas(
			PlayerTemplate->GetCharacterAttributes()->GetMaxBalas() + CantidadAumentar);
		
		PlayerTemplate->GetCharacterAttributes()->SetBalasActuales(
			PlayerTemplate->GetCharacterAttributes()->GetMaxBalas());
	}
}
