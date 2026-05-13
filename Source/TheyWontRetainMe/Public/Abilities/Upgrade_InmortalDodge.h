#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Upgrade_InmortalDodge.generated.h"

/**
 * Mejora Ability_Dodge para que ahora seas inmortal mientras esquivas.
 */
UCLASS()
class THEYWONTRETAINME_API UUpgrade_InmortalDodge : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
};
