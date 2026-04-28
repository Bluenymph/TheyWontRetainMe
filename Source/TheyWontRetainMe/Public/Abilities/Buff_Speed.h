#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Buff_Speed.generated.h"

/**
 * Habilidad para aumentar la velocidad base del player
 */
UCLASS()
class THEYWONTRETAINME_API UBuff_Speed : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IncrementoPorcentual = 0.3f;
private:
	const float MAX_SPEED = 1200.f;
};
