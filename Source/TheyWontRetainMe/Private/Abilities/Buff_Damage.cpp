#include "Abilities/Buff_Damage.h"

#include "Characters/PlayerTemplate.h"

void UBuff_Damage::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate)
	{
		PlayerTemplate->SetPotenciaDisparo(PlayerTemplate->GetPotenciaDisparo() + CantidadAumentar);
	}

}
