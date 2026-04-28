#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "ExtraBullet.generated.h"

/**
 * Habilidad que suma una instancia de bala más por cada disparo.
 */
UCLASS()
class THEYWONTRETAINME_API UExtraBullet : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CantidadAumentar = 1;
};
