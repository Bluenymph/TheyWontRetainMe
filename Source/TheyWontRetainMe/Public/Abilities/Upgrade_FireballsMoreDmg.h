#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Upgrade_FireballsMoreDmg.generated.h"

/**
 * Mejora de la habilidad Ability_Fireballs para aumentar su daño
 */
UCLASS()
class THEYWONTRETAINME_API UUpgrade_FireballsMoreDmg : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CantidadAumentar = 10.f;
};
