#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ModoJuego.generated.h"

class UAbilitiesDataBase;
class APlayerTemplate;
class AEnemyTemplate;
class ABulletTemplate;
class UUserWidget;
class UBulletPoolSubsystem;
class UEnemiesManager;


/**
 * El modo de juego por defecto.
 * AModoJuego* ModoJuego = Cast<AModoJuego>(UGameplayStatics::GetGameMode(GetWorld()));
 */
UCLASS()
class THEYWONTRETAINME_API AModoJuego : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;
	
	UFUNCTION()
	FORCEINLINE TSubclassOf<UUserWidget> GetImpactInfoClass() { return ImpactInfoClass; }
	
protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<ABulletTemplate>>  BulletsToLoad;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<AEnemyTemplate>>  EnemiesToLoad;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAbilitiesDataBase* SkillsDataBase;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> ImpactInfoClass;

private:
	FTimerHandle TimerHandle_BeginSpawnEnemies;
	
	UPROPERTY()
	UBulletPoolSubsystem* BulletSubsystem;
	
	UPROPERTY()
	UEnemiesManager* EnemiesManager;
	
	UFUNCTION()
	void PrewarmPools();
	
	UFUNCTION()
	void SpawnEnemies();
};
