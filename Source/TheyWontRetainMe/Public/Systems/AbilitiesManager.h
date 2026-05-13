#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AbilitiesManager.generated.h"

class UAbilitiesDataBase;
class UAbilityData;
class UBaseAbility;

/*
 * Sistema para manejar la lógica de las habilidades de manera centralizada.
 * UAbilitiesManager* AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
 */
UCLASS()
class THEYWONTRETAINME_API UAbilitiesManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void LoadAllAbilities(UAbilitiesDataBase* DataBase);
	
	UPROPERTY()
	TArray<UBaseAbility*> ActiveAbilities;
	
	UPROPERTY()
	TArray<UAbilityData*> AllAbilitiesAvailable;
	
	UPROPERTY()
	TMap<UAbilityData*, int32> AbilitiesStack;
	
	UFUNCTION()
	void TickAbilities();
	
	UFUNCTION(BlueprintCallable)
	void AddAbilityFromData(UAbilityData* AbilityData);
	
	//Busca la habilidad en las que estan activas por UAbilityData, si no la encuentra devuelve -1
	UFUNCTION(BlueprintCallable)
	int FindAbility(const UAbilityData* AbilityData);
	
	//Lo mismo que arriba pero por nombre
	UFUNCTION(BlueprintCallable)
	int FindAbilityByName(FText Name);
	
private:
	FTimerHandle TimerHandle_TickAbilities;
	
	UPROPERTY()
	bool bSlowEnemies = false;
	
	UPROPERTY()
	bool bAbility_InmortalDodge = false;
	
	UPROPERTY()
	bool bAbility_ReloadDodge = false;
	
	UPROPERTY()
	bool bDodgeUnlocked = false;
	
	UPROPERTY()
	int ExtraBulletsQuantity = 0;
	
public:
	FORCEINLINE bool GetSlowEnemies() const  { return bSlowEnemies; }
	FORCEINLINE bool GetAbility_InmortalDodge() const { return bAbility_InmortalDodge; }
	FORCEINLINE bool GetAbility_ReloadDodge() const { return bAbility_ReloadDodge; }
	FORCEINLINE bool GetDodgeUnlocked() const { return bDodgeUnlocked; }
	FORCEINLINE int GetExtraBulletsQuantity() const { return ExtraBulletsQuantity; }
	
	FORCEINLINE void SetSlowEnemies(const bool Value) { bSlowEnemies = Value; }
	FORCEINLINE void SetAbility_InmortalDodge(const bool Value) { bAbility_InmortalDodge = Value; }
	FORCEINLINE void SetAbilityReloadDodge(const bool Value) { bAbility_ReloadDodge = Value; }
	FORCEINLINE void SetDodgeUnlocked(const bool Value) { bDodgeUnlocked = Value; }
	FORCEINLINE void SetExtraBulletsQuantity(const int Value) { ExtraBulletsQuantity = Value; }
	
};
