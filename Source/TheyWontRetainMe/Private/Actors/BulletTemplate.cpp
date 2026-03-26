#include "Actors/BulletTemplate.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Systems/BulletPoolSubsystem.h"

ABulletTemplate::ABulletTemplate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = CollisionComp;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bAutoActivate = false;
}

void ABulletTemplate::OnActivateBullet_Implementation(FVector ShootDirection, float Speed)
{
	IBulletInterface::OnActivateBullet_Implementation(ShootDirection, Speed);
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	ProjectileMovement->SetUpdatedComponent(CollisionComp);
	
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->Velocity = ShootDirection * Speed;
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
	//AQUI EL CODIGO CUANDO CHOQUE CON ALGO
	
	OnDeactivateBullet();
}
