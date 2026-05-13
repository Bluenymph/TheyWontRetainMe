#include "Actors/BulletTemplate.h"
#include "LogMacros.h"
#include "Characters/EnemyTemplate.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/HiteableInterface.h"
#include "Interfaces/EnemyInterface.h"
#include "Systems/AbilitiesManager.h"
#include "Systems/BulletPoolSubsystem.h"
#include "Systems/EnemiesManager.h"
#include "Systems/GameManager.h"

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

void ABulletTemplate::BeginPlay()
{
	Super::BeginPlay();
	
	GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
	OriginalScale = GetActorScale3D();
}

void ABulletTemplate::OnActivateBullet_Implementation(FVector ShootDirection, float Damage, float Speed, AActor* HitOwner, float Crit)
{
	IBulletInterface::OnActivateBullet_Implementation(ShootDirection, Damage, Speed, HitOwner, Crit);

	UEnemiesManager* Manager = GetWorld()->GetSubsystem<UEnemiesManager>();
	AEnemyTemplate* TargetEnemy = Manager->GetEnemyUnderTarget();
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	ProjectileMovement->SetUpdatedComponent(CollisionComp);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	CritChance = Crit;
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
	
	
	BulletOwner = HitOwner;
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
	CritChance = 0.f;
	BulletOwner = nullptr;
	
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
	if (OtherActor->GetClass()->ImplementsInterface(UHiteableInterface::StaticClass()) && BulletOwner)
	{
		IHiteableInterface::Execute_OnHitReceived(OtherActor,BulletDamage, BulletOwner);
	}
	
	if (OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
	{
		if (GetWorld()->GetSubsystem<UAbilitiesManager>()->GetSlowEnemies() == true)
		{
			IEnemyInterface::Execute_OnSlowEnemy(OtherActor,1.f);
		}
	}
	
	float Probabilidad = FMath::FRandRange(0.0f, 100.0f);

	if (Probabilidad <= CritChance * 100.0f)
	{
		BulletDamage *= 2;
		LOG("GOLPE CRITICO MAN")
	}
	
	GameManager->ShowImpacNumber(GetActorLocation(),this,BulletDamage);
	OnDeactivateBullet_Implementation();
}