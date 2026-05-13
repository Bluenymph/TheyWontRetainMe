#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Buff_Damage.generated.h"

/**
 * Aumentan el daño de las balas del player.
 */
UCLASS()
class THEYWONTRETAINME_API UBuff_Damage : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CantidadAumentar;
};
