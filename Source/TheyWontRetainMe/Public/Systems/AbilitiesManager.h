#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AbilitiesManager.generated.h"

class UAbilityData;
class UBaseAbility;

UCLASS()
class THEYWONTRETAINME_API UAbilitiesManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UPROPERTY()
	float CurrentExperience;
	
	UPROPERTY()
	float NextLevelExp;
	
	UPROPERTY()
	TArray<UBaseAbility*> ActiveAbilities;
	
	UFUNCTION()
	void NextLevel(); //Se llama para calcular cuanto sera el siguiente nivel en exp
	
	UFUNCTION()
	void TickAbilities();
	
	UFUNCTION(BlueprintCallable)
	void AddAbilityFromData(const UAbilityData* AbilityData);
	
private:
	FTimerHandle TimerHandle_TickAbilities;
	
	UFUNCTION()
	int FindAbility(const UAbilityData* AbilityData);
	
};
