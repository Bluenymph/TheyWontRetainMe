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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyDamaged, AEnemyTemplate*, Enemy, float, Damage);
/**
 * Sistema para instanciar y manejar a los enemigos.
 * Vamos a usar el protocolo de PoolObjects. Instanciaremos 1000 enemigos al principio y los iremos reciclando.
 */
UCLASS()
class THEYWONTRETAINME_API UEnemiesManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	UFUNCTION(BlueprintCallable)
	void PrewarmEnemyPool(TSubclassOf<AEnemyTemplate> EnemyClass, int32 Amount); 

	//Coge un enemigo del pool. Si no hay, lo crea.
	UFUNCTION(BlueprintCallable)
	AEnemyTemplate* GetEnemyFromPool(TSubclassOf<AEnemyTemplate> EnemyClass, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void ReturnEnemyToPool(AEnemyTemplate* Enemy, bool bGiveExp);

	UFUNCTION(BlueprintCallable)
	void BeginManageEnemiesLoop();

	UFUNCTION(BlueprintCallable)
	void ManageEnemies();

	UFUNCTION(BlueprintCallable)
	AEnemyTemplate* GetEnemyUnderTarget();
	
	UFUNCTION(BlueprintCallable)
	FVector GetNearestEnemy(FVector ActorPosition);
	
	UFUNCTION(BlueprintCallable)
	void OnEnemyBeginDamaged(AEnemyTemplate* Enemy, float Damage);

	void Deinitialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRadius = 1500.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<AEnemyTemplate>>  EnemiesToSpawn;
	
	UPROPERTY()
	int32 EnemiesAttackTokens = 4; //Los enemigos que van a intentar atacar a la vez
	
	UPROPERTY()
	int32 LimitEnemies = 499;
	
	UPROPERTY()
	FOnEnemyDamaged OnEnemyDamaged;

private:
	FTimerHandle TimerHandle_ManageEnemies;
	FTimerHandle TimerHandle_GameRhythm;
	
	const FVector UnderGroundLocation = FVector(0.0f, 0.0f, -100.0f);
	
	UPROPERTY()
	APawn* PlayerPawn;
	
	UPROPERTY()
	UGameManager* GameManager;
	
	UPROPERTY()
	int32 ToMuchEnemies = 20; //Numero para que el codigo sepa si se esta pasando con el spawn
	
	UPROPERTY()
	float HealthExponential = 0.0f;
	
	UPROPERTY()
	float InitialHealth = 60;
	
	UPROPERTY()
	int32 NextSpawnTokenThreshold = 10;//Segundos 

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
	
	UFUNCTION()
	void UpdateGameRhythm();
	
};
