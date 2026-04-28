#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BulletPoolSubsystem.generated.h"

class ABulletTemplate;
class AModoJuego;

/**
 * Hago este struct por si en el futuro queremos meter varios tipos de bala.
 * Asi puedo hacer varias pool por tipo y meterlas en el map de despues.
 */
USTRUCT()
struct FBulletPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<ABulletTemplate*> InactiveBullets;
};

/**
 * Sistema para "instanciar" balas a tutiplen.
 * Vamos a usar el protocolo de PoolObjects. Instanciaremos 500 balas al principio y las iremos reciclando.
 */
UCLASS()
class THEYWONTRETAINME_API UBulletPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PrewarmPool(TSubclassOf<ABulletTemplate> BulletClass, int32 Amount); //Instanciamos unas cuantas balas al principio
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABulletTemplate* GetBulletFromPool(TSubclassOf<ABulletTemplate> BulletClass, FVector Location, FRotator Rotation, float Damage, AActor* HitOwner);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ReturnBulletToPool(ABulletTemplate* Bullet);

private:
	UPROPERTY()
	TMap<TSubclassOf<ABulletTemplate>, FBulletPool> PoolMap;
	
	UPROPERTY()
	AModoJuego* ModoJuego;
};