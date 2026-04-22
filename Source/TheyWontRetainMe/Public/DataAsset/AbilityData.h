#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityData.generated.h"

UCLASS()
class THEYWONTRETAINME_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<UBaseAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bIsUnique;
};
