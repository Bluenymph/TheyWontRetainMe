#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RandomImpactAbility.generated.h"

UINTERFACE()
class URandomImpactAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * Esta interfaz se aplica a las habilidades que spawneen aleatoriamente al darse un efecto de impacto.
 */
class THEYWONTRETAINME_API IRandomImpactAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TrySpawnAbility(FVector Position);
};
