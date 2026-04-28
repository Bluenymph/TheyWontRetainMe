#include "Abilities/Buff_ShootSpeed.h"
#include "Characters/PlayerTemplate.h"

void UBuff_ShootSpeed::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate)
	{
		if (PlayerTemplate->GetCadenciaDisparo() <= 0.1) return;
		PlayerTemplate->SetCadenciaDisparo(PlayerTemplate->GetCadenciaDisparo() - CantidadVelocidad);
	}
}
