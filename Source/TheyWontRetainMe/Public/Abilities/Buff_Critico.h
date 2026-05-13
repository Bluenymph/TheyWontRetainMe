#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Buff_Critico.generated.h"

/**
 * Habilidad para aumentar el porcentaje de critico
 */
UCLASS()
class THEYWONTRETAINME_API UBuff_Critico : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CantidadAumentar = 0.1f;
};
