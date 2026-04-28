#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Buff_MoreAmmo.generated.h"

/**
 *  Aumenta la cantidad de municion.
 */
UCLASS()
class THEYWONTRETAINME_API UBuff_MoreAmmo : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CantidadAumentar = 5;
};
