#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Esta interfaz manejara el spawn y despawn de los enemigos.
 */
class THEYWONTRETAINME_API IEnemyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnActivateEnemy(FVector Position, FRotator Rotation);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDeactivateEnemy(bool bGiveExp);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSlowEnemy(float TimeAmount);
};
