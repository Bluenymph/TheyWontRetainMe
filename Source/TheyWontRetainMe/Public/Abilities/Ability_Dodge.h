#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_Dodge.generated.h"

/**
 * Habilidad para desbloquear el salto.
 */
UCLASS()
class THEYWONTRETAINME_API UAbility_Dodge : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
};
