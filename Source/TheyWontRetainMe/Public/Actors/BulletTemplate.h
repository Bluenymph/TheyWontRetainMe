#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BulletInterface.h"
#include "BulletTemplate.generated.h"

class UGameManager;
class USphereComponent;
class UProjectileMovementComponent;
class UUserWidget;

UCLASS()
class THEYWONTRETAINME_API ABulletTemplate : public AActor, public IBulletInterface
{
	GENERATED_BODY()
	
public:	
	ABulletTemplate();
	
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

	virtual void OnActivateBullet_Implementation(FVector ShootDirection, float Damage, float Speed, AActor* HitOwner, float Crit) override;
	virtual void OnDeactivateBullet_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void AutoReturnToPool();
	
	FTimerHandle LifeTimerHandle;

	UPROPERTY(EditAnywhere)
	float MaxLifeTime = 0.5f; 

	UFUNCTION(BlueprintCallable)
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY()
	float BulletDamage = 1.f;
	
	UPROPERTY()
	AActor* BulletOwner;
	
	UPROPERTY()
	UGameManager* GameManager;
	
	UPROPERTY()
	UAbilitiesManager* AbilitiesManager;
	
	UPROPERTY()
	FVector OriginalScale;
	
	UPROPERTY()
	float CritChance = 0.f; 

public:
	FORCEINLINE void SetBulletDamage(float Damage) { BulletDamage = Damage; }
	
	FORCEINLINE FVector GetOriginalScale() const { return OriginalScale; }
	
	FORCEINLINE void ReduceBulletSize(float Divisor) { SetActorScale3D(GetActorScale3D() / Divisor); }
};