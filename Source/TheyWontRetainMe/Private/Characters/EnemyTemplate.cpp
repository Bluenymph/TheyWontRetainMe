#include "Characters/EnemyTemplate.h"
#include "LogMacros.h"
#include "AnimInstances/PecadorAnimInstance.h"
#include "Characters/PlayerTemplate.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterAttributes.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Systems/EnemiesManager.h"
#include "Systems/GameManager.h"

AEnemyTemplate::AEnemyTemplate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	
	HitComponent = CreateDefaultSubobject<UBoxComponent>("HitComponent");
	HitComponent->SetupAttachment(SkeletalMeshComponent, "WeaponSocket");
	HitComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributes>("Atributos");

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

void AEnemyTemplate::BeginPlay()
{
	Super::BeginPlay();
	
	PecadorAnimInstance = Cast<UPecadorAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
	
	Velocidad = MaxVelocidad;
	GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
	HitComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTemplate::OnWeaponOverlap);
}

void AEnemyTemplate::OnHitReceived_Implementation(float Damage, AActor* HitInstigator)
{
	IHiteableInterface::OnHitReceived_Implementation(Damage, HitInstigator);
	if (CharacterAttributes->AttributesTakeDmg(Damage) < 0.f)
	{
		OnDeactivateEnemy_Implementation();
	}
	PecadorAnimInstance->OnAnimEnemyAttacked(CharacterAttributes->GetVidaActual());
	
	if (GameManager && HitInstigator->ActorHasTag("Player")) GameManager->CurrentPlayer->OnDamageInflicted(Damage);
}

void AEnemyTemplate::OnActivateEnemy_Implementation(FVector Position, FRotator Rotation)
{
	IEnemyInterface::OnActivateEnemy_Implementation(Position, Rotation);
	
	PecadorAnimInstance->SetBehaviourState(EEnemyBehaviourState::EEBS_Idle);
	SkeletalMeshComponent->SetComponentTickEnabled(true);
	SkeletalMeshComponent->Activate();
	
	SetActorLocationAndRotation(Position, Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AEnemyTemplate::OnSlowEnemy_Implementation(float TimeAmount)
{
	IEnemyInterface::OnSlowEnemy_Implementation(TimeAmount);
	
	Velocidad = MaxVelocidad/3;
	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(TimerHandle_Slow,this,&AEnemyTemplate::OnSlowCD,TimeAmount,false);
}

void AEnemyTemplate::OnDeactivateEnemy_Implementation()
{
	IEnemyInterface::OnDeactivateEnemy_Implementation();

	SkeletalMeshComponent->SetComponentTickEnabled(false);
	SkeletalMeshComponent->Deactivate();
	
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

	float CurrentDistance = FVector::Dist(Player->GetActorLocation(), CurrentLoc);

	if (CurrentDistance > StoppingDistance)
	{
		FVector NextLocation = CurrentLoc + (Direction * Velocidad * DeltaTime);
		FVector GroundNormal;
		
		AdjustLocationToGround(NextLocation, GroundNormal);
		AdjustRotationToGround(Direction, GroundNormal, DeltaTime);

		SetActorLocation(NextLocation, true);
	}else
	{
		Velocidad = 300.f;
		PecadorAnimInstance->OnAnimEnemyAttack();
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

void AEnemyTemplate::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		IHiteableInterface::Execute_OnHitReceived(OtherActor,15.f, this);
	}
}

void AEnemyTemplate::OnSlowCD()
{
	Velocidad = MaxVelocidad;
}
