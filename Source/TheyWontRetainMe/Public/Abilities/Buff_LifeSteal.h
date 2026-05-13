#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Buff_LifeSteal.generated.h"

/**
 * Habilidad para incluir robo de vida cada vez que el player en persona haga daño.
 * No se incluye invocaciones.
 */
UCLASS()
class THEYWONTRETAINME_API UBuff_LifeSteal : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AcumulacionRoboVida = 0.2f;
	
private:
	const float MAX_LIFESTEAL = 0.8f;
};
