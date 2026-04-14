#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HiteableInterface.generated.h" 

UINTERFACE(MinimalAPI)
class UHiteableInterface : public UInterface
{
	GENERATED_BODY()
};

class THEYWONTRETAINME_API IHiteableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHitReceived(float Damage);
};