#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_Torreta.generated.h"

class ATurretActor;
class UEnemiesManager;

/**
 * Habilidad que pone una torreta en el suelo para que dispare automaticamente.
 */
UCLASS()
class THEYWONTRETAINME_API UAbility_Torreta : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<ATurretActor> VisualActorClass;
	
	virtual void ActivateAbility(AActor* InOwner) override;
	virtual void TickAbility(float DeltaTime) override;
	
	UFUNCTION()
	void TorretaShoot();
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Cadencia = 0.8f;
	
private:
	FTimerHandle FTimerHandler_Shoot;
	
	UPROPERTY()
	UEnemiesManager* EnemiesManager;
	
	UPROPERTY()
	ATurretActor* TurretActor;
};
