#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Nerf_SlowEnemies.generated.h"

/**
 * Habilidad para nerfear la velocidad de los enemigos al dispararles.
 */
UCLASS()
class THEYWONTRETAINME_API UNerf_SlowEnemies : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
	
};
