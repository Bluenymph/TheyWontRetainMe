#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset/AbilityData.h"
#include "AbilitiesDataBase.generated.h"

UCLASS()
class THEYWONTRETAINME_API UAbilitiesDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAbilityData*> AllAbilities;
	
};
