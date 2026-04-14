#include "Actors/BulletTemplate.h"
#include "LogMacros.h"
#include "Characters/EnemyTemplate.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/HiteableInterface.h"
#include "Systems/BulletPoolSubsystem.h"
#include "Systems/EnemiesManager.h"

ABulletTemplate::ABulletTemplate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = CollisionComp;
	CollisionComp->OnComponentHit.AddDynamic(this, &ABulletTemplate::OnHit);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bAutoActivate = false;
}

void ABulletTemplate::OnActivateBullet_Implementation(FVector ShootDirection, float Damage, float Speed)
{
	IBulletInterface::OnActivateBullet_Implementation(ShootDirection, Damage, Speed);

	UEnemiesManager* Manager = GetWorld()->GetSubsystem<UEnemiesManager>();
	AEnemyTemplate* TargetEnemy = Manager->GetEnemyUnderTarget();
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	ProjectileMovement->SetUpdatedComponent(CollisionComp);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	BulletDamage = Damage;

	if (TargetEnemy)
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingAccelerationMagnitude = 4000.f;
		ProjectileMovement->HomingTargetComponent = TargetEnemy->GetRootComponent();
		
		FVector ToTarget = (TargetEnemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector ModifiedDirection = ShootDirection;

		ModifiedDirection.Z += (ToTarget.Z - ShootDirection.Z) * 0.5f; // 0.5 es tu multiplicador de ayuda
		ModifiedDirection.Normalize();

		ProjectileMovement->Velocity = ModifiedDirection * Speed;
	}else
	{
		ProjectileMovement->Velocity = ShootDirection * Speed;
	}
	
	
	ProjectileMovement->Activate();
	
	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &ABulletTemplate::AutoReturnToPool, MaxLifeTime, false);
}

void ABulletTemplate::OnDeactivateBullet_Implementation()
{
	IBulletInterface::OnDeactivateBullet_Implementation();
	
	SetActorLocation(FVector(-999999.f, -999999.f, -999999.f));
	
	ProjectileMovement->Deactivate();
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->SetUpdatedComponent(nullptr);
	ProjectileMovement->bIsHomingProjectile = false;
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ABulletTemplate::AutoReturnToPool()
{
	if (auto* Pool = GetWorld()->GetSubsystem<UBulletPoolSubsystem>())
	{
		Pool->ReturnBulletToPool(this);
	}
}

void ABulletTemplate::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	OnDeactivateBullet_Implementation();
	if (OtherActor->GetClass()->ImplementsInterface(UHiteableInterface::StaticClass()))
	{
		IHiteableInterface::Execute_OnHitReceived(OtherActor,BulletDamage);
	}
	
}
