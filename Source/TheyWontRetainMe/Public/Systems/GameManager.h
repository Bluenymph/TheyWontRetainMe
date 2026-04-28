#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

class AModoJuego;
class APlayerTemplate;
class UBaseAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, int, CurrentLevel, int, Levels);

/**
 *Usaremos esto para guardar datos entre saltos de mapas.
 * UGameManager* GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
 */
UCLASS()
class THEYWONTRETAINME_API UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	APlayerTemplate* CurrentPlayer = nullptr;
	
	UPROPERTY()
	FOnLevelUp OnLevelUp;
	
	UFUNCTION()
	void ChangeScene();
	
	UFUNCTION()
	void AddExp(float Quantity);
	
	UFUNCTION()
	void ShowImpacNumber(FVector ImpactPoint, AActor* InstigatorActor, float Damage);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	
	FORCEINLINE float GetCurrentEXP() const { return CurrentEXP; }
	FORCEINLINE int32 GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE float GetNextLevelThreshold() const { return NextLevelThreshold; }

	FORCEINLINE void SetCurrentEXP(float NewEXP) { CurrentEXP = NewEXP; }
	FORCEINLINE void SetCurrentLevel(int32 NewLevel) { CurrentLevel = NewLevel; }
	FORCEINLINE void SetNextLevelThreshold(float NewThreshold) { NextLevelThreshold = NewThreshold; }
	
	
private:
	const float EXP_MULTIPLIER = 1.15f;
	const float BASE_XP = 20.f;
	
	UPROPERTY()
	float CurrentEXP = 0.f;
	
	UPROPERTY()
	int CurrentLevel = 0;
	
	UPROPERTY()
	float NextLevelThreshold;
	
	UPROPERTY()
	int32 LevelsToUp = 0;
	
	UPROPERTY()
	AModoJuego* ModoJuego;
	
	
#pragma region SaveData
	
	UPROPERTY()
	TArray<UBaseAbility*> CurrentActiveAbilities;
	
#pragma endregion SaveData
	
	
	
	
};
