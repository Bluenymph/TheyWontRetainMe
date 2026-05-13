#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BulletInterface.generated.h" 

UINTERFACE(MinimalAPI)
class UBulletInterface : public UInterface
{
	GENERATED_BODY()
};

class THEYWONTRETAINME_API IBulletInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnActivateBullet(FVector ShootDirection, float Damage, float Speed, AActor* Owner, float Crit);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDeactivateBullet();
};