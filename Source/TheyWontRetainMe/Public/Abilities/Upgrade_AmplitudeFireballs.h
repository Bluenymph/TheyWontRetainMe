#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "UObject/Object.h"
#include "Upgrade_AmplitudeFireballs.generated.h"

/**
 * Habilidad que mejora a Ability_Fireballs para que su radio sea mayor, haciendo que orbiten mas lejos.
 */
UCLASS()
class THEYWONTRETAINME_API UUpgrade_AmplitudeFireballs : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
};
