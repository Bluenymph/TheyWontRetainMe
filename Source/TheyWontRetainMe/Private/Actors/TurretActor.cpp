#include "Actors/TurretActor.h"
#include "LogMacros.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Systems/BulletPoolSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Systems/AbilitiesManager.h"


ATurretActor::ATurretActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	SetRootComponent(CapsuleCollision);
	
	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScopeMesh");
	ScopeMesh->SetupAttachment(RootComponent);
	
	BodyTurret = CreateDefaultSubobject<UStaticMeshComponent>("BodyTurret");
	BodyTurret->SetupAttachment(RootComponent);
	
	PointToShoot = CreateDefaultSubobject<USceneComponent>("PointToShoot");
	PointToShoot->SetupAttachment(ScopeMesh);
}

void ATurretActor::RotateTurret(FVector NewPosition)
{
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewPosition);
	LookAt.Yaw += 90.0f;
	FQuat RelQuat = GetActorTransform().InverseTransformRotation(LookAt.Quaternion());
	ScopeMesh->SetRelativeRotation(RelQuat.Rotator());
}

void ATurretActor::Shoot(float Damage, TSubclassOf<ABulletTemplate> BulletClass)
{
	FRotator NuevaRotacion = ScopeMesh->GetComponentRotation();
	NuevaRotacion.Yaw -= 90.0f;
	
	BulletPoolSubsystem->GetBulletFromPool(BulletClass, 
		PointToShoot->GetComponentLocation(),
		NuevaRotacion,
		Damage,
		this
		);
}

void ATurretActor::InitDestructionTimer(float Time)
{
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_LifeTime,
		this,
		&ATurretActor::AutoDestruction,
		Time,
		false);
}

void ATurretActor::AutoDestruction()
{
	if (!AbilitiesManager) return;
	
	int32 TurretPosList = AbilitiesManager->FindAbilityByName(FText::FromString("Torreta infernal"));
	if (TurretPosList > -1)
	{
		AbilitiesManager->ActiveAbilities.RemoveAt(TurretPosList);
	}
	this->Destroy();
}

void ATurretActor::BeginPlay()
{
	Super::BeginPlay();
	
	BulletPoolSubsystem = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
	AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
}


