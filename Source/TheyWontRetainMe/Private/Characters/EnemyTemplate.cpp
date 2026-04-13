#include "Characters/EnemyTemplate.h"
#include "LogMacros.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterAttributes.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Systems/EnemiesManager.h"

AEnemyTemplate::AEnemyTemplate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CapsuleComponent);

	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributes>("Atributos");

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

void AEnemyTemplate::OnHitReceived_Implementation(float Damage)
{
	IHiteableInterface::OnHitReceived_Implementation(Damage);
	if (CharacterAttributes->AttributesTakeDmg(Damage) < 0.f)
		OnDeactivateEnemy_Implementation();
}

void AEnemyTemplate::OnActivateEnemy_Implementation(FVector Position, FRotator Rotation)
{
	IEnemyInterface::OnActivateEnemy_Implementation(Position, Rotation);
	
	SetActorLocationAndRotation(Position, Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AEnemyTemplate::OnDeactivateEnemy_Implementation()
{
	IEnemyInterface::OnDeactivateEnemy_Implementation();

	SetActorLocation(FVector(0, 0, -50000.f)); 
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	UEnemiesManager* EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	if (EnemiesManager) EnemiesManager->ReturnEnemyToPool(this);
}

void AEnemyTemplate::UpdateMovement(APawn* Player, float DeltaTime)
{
	if (!Player) return;

	FVector CurrentLoc = GetActorLocation();
	FVector Direction = (Player->GetActorLocation() - CurrentLoc).GetSafeNormal();
	Direction.Z = 0; 

	if (FVector::Dist(Player->GetActorLocation(), CurrentLoc) > StoppingDistance)
	{
		FVector NextLocation = CurrentLoc + (Direction * Speed * DeltaTime);
		FVector GroundNormal;
		
		AdjustLocationToGround(NextLocation, GroundNormal);
		AdjustRotationToGround(Direction, GroundNormal, DeltaTime);

		SetActorLocation(NextLocation, true);
		//SetActorRotation(Direction.Rotation());
	}
}

void AEnemyTemplate::AdjustLocationToGround(FVector& OutLocation, FVector& OutNormal)
{
	FVector TraceStart = OutLocation + FVector(0.f, 0.f, 100.f);
	FVector TraceEnd = OutLocation - FVector(0.f, 0.f, 500.f);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		float Offset = 0.9f; 
		if (UCapsuleComponent* Capsule = GetRootComponent()->IsA(UCapsuleComponent::StaticClass()) ? Cast<UCapsuleComponent>(GetRootComponent()) : nullptr)
		{
			Offset = Capsule->GetScaledCapsuleHalfHeight() + ZOffset;
		}
        
		OutLocation.Z = Hit.ImpactPoint.Z + Offset;
		OutNormal = Hit.Normal;
	}
	else 
	{
		OutNormal = FVector::UpVector;
	}
}

void AEnemyTemplate::AdjustRotationToGround(const FVector& Direction, const FVector& GroundNormal, float DeltaTime)
{
	FRotator TargetRotation = FRotationMatrix::MakeFromZX(GroundNormal, Direction).Rotator();
	FRotator SmoothRot = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.f);
    
	SetActorRotation(SmoothRot);
}
