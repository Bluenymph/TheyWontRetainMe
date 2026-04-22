#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityVisualInterface.generated.h"

UINTERFACE(Blueprintable)
class UAbilityVisualInterface : public UInterface
{
	GENERATED_BODY()
};

class THEYWONTRETAINME_API IAbilityVisualInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void SetParentAbility(UObject* NewParent);
};
