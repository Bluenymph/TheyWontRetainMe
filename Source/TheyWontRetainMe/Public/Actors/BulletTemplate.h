#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BulletInterface.h"
#include "BulletTemplate.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class THEYWONTRETAINME_API ABulletTemplate : public AActor, public IBulletInterface
{
	GENERATED_BODY()
	
public:	
	ABulletTemplate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

	virtual void OnActivateBullet_Implementation(FVector ShootDirection, float Damage, float Speed) override;
	virtual void OnDeactivateBullet_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void AutoReturnToPool();
	
	FTimerHandle LifeTimerHandle;

	UPROPERTY(EditAnywhere)
	float MaxLifeTime = 1.0f; 

	UFUNCTION(BlueprintCallable)
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY()
	float BulletDamage = 1.f;
};