#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "BigHeal.generated.h"

/**
 * Habilidad de curacion completa.
 */
UCLASS()
class THEYWONTRETAINME_API UBigHeal : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
};
