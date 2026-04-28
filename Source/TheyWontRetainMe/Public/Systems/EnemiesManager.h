#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemiesManager.generated.h"

class UGameManager;
class AEnemyTemplate;

/**
 * Hago este struct por si en el futuro queremos meter varios tipos de enemigos.
 */
USTRUCT()
struct FEnemyPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AEnemyTemplate*> InactiveEnemies;
};

/**
 * Sistema para instanciar y manejar a los enemigos.
 * Vamos a usar el protocolo de PoolObjects. Instanciaremos 1000 enemigos al principio y los iremos reciclando.
 */
UCLASS()
class THEYWONTRETAINME_API UEnemiesManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PrewarmEnemyPool(TSubclassOf<AEnemyTemplate> EnemyClass, int32 Amount); 

	//Coge un enemigo del pool. Si no hay, lo crea.
	UFUNCTION(BlueprintCallable)
	AEnemyTemplate* GetEnemyFromPool(TSubclassOf<AEnemyTemplate> EnemyClass, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void ReturnEnemyToPool(AEnemyTemplate* Enemy);

	UFUNCTION(BlueprintCallable)
	void BeginManageEnemiesLoop();

	UFUNCTION(BlueprintCallable)
	void ManageEnemies();

	UFUNCTION(BlueprintCallable)
	AEnemyTemplate* GetEnemyUnderTarget();
	
	UFUNCTION(BlueprintCallable)
	FVector GetNearestEnemy(FVector ActorPosition);

	void Deinitialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRadius = 1500.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<AEnemyTemplate>>  EnemiesToSpawn;

private:
	FTimerHandle TimerHandle_ManageEnemies;
	
	UPROPERTY()
	APawn* PlayerPawn;
	
	UPROPERTY()
	UGameManager* GameManager;

#pragma region SpawnEnemiesLoop
	FTimerHandle LoopEnemySpawn;
	
	UFUNCTION()
	void SpawnEnemiesLoop();

	UFUNCTION()
	void StopSpawnEnemiesLoop();

	UFUNCTION()
	bool GetValidSpawnLocation(FVector& OutLocation, FRotator& OutRotation);
#pragma endregion 
	
	UPROPERTY()
	TMap<TSubclassOf<AEnemyTemplate>, FEnemyPool> EnemyPoolMap;

	UPROPERTY()
	TArray<AEnemyTemplate*> ActiveEnemies;
};
