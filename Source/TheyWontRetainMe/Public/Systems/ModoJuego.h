#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ModoJuego.generated.h"

class APlayerTemplate;
class AEnemyTemplate;
class ABulletTemplate;

/**
 * El modo de juego por defecto.
 */
UCLASS()
class THEYWONTRETAINME_API AModoJuego : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<ABulletTemplate>>  BulletsToLoad;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<AEnemyTemplate>>  EnemiesToLoad;

};
