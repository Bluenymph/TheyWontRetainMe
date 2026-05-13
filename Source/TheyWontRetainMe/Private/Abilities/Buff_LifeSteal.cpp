#include "Abilities/Buff_LifeSteal.h"

#include "Characters/PlayerTemplate.h"


void UBuff_LifeSteal::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate)
	{
		PlayerTemplate->SetRoboVidaIndicador(FMath::Lerp(
			PlayerTemplate->GetRoboVidaIndicador(),
			MAX_LIFESTEAL,
			AcumulacionRoboVida));
	}
}
