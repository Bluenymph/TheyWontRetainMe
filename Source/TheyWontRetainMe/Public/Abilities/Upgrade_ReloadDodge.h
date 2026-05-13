#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Upgrade_ReloadDodge.generated.h"

/**
 * Abilidad que mejora Ability_Dodge para que cuando esquivas se recargue el arma.
 */
UCLASS()
class THEYWONTRETAINME_API UUpgrade_ReloadDodge : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
};
