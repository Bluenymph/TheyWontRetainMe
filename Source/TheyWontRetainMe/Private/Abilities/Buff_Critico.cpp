#include "Abilities/Buff_Critico.h"

#include "LogMacros.h"
#include "Characters/PlayerTemplate.h"

void UBuff_Critico::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate)
	{
		PlayerTemplate->SetProbabilidadCritico(PlayerTemplate->GetProbabilidadCritico()+CantidadAumentar);
		LOG("Critico aumentado a: %f", PlayerTemplate->GetProbabilidadCritico());
	}
}
