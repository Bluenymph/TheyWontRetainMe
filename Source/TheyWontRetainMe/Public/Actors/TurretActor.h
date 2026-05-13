#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretActor.generated.h"

class UAbilitiesManager;
class ABulletTemplate;
class UBulletPoolSubsystem;
class UStaticMeshComponent;
class USceneComponent;
class UCapsuleComponent;

UCLASS()
class THEYWONTRETAINME_API ATurretActor : public AActor
{
	GENERATED_BODY()

public:
	ATurretActor();
	
	UFUNCTION(BlueprintCallable)
	void RotateTurret(FVector NewPosition);
	
	UFUNCTION(BlueprintCallable)
	void Shoot(float Damage, TSubclassOf<ABulletTemplate> BulletClass);
	
	UFUNCTION(BlueprintCallable)
	void InitDestructionTimer(float Time);

	UFUNCTION(BlueprintCallable)
	void AutoDestruction();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* PointToShoot;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCapsuleComponent* CapsuleCollision;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* ScopeMesh;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* BodyTurret;
	
private:
	FTimerHandle TimerHandle_LifeTime;

	UPROPERTY()
	UBulletPoolSubsystem* BulletPoolSubsystem;
	
	UPROPERTY()
	UAbilitiesManager* AbilitiesManager;

};
