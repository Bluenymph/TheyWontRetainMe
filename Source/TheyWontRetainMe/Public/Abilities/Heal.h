#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Heal.generated.h"

/**
 * Habilidad que cura una pequeña cantidad al jugador.
 */
UCLASS()
class THEYWONTRETAINME_API UHeal : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CantidadCura = 30;
};
