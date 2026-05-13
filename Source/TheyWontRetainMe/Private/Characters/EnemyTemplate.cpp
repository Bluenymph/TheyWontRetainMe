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
#include "PaperFlipbookComponent.h"
#include "ToolContextInterfaces.h"
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
	EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	HitComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTemplate::OnWeaponOverlap);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	PortalActor = GetWorld()->SpawnActor<AActor>(
		PortalActorClass,
		GetActorLocation(),
		GetActorRotation(),
		SpawnParams);
	
	if (PortalActor) PortalFlipbookComponent = PortalActor->FindComponentByClass<UPaperFlipbookComponent>();
	if (PortalFlipbookComponent)
	{
		PortalFlipbookComponent->SetUsingAbsoluteLocation(true);
		PortalFlipbookComponent->SetUsingAbsoluteRotation(true);
		PortalFlipbookComponent->SetUsingAbsoluteScale(true);
	}
}

void AEnemyTemplate::OnHitReceived_Implementation(float Damage, AActor* HitInstigator)
{
	IHiteableInterface::OnHitReceived_Implementation(Damage, HitInstigator);
	if (CharacterAttributes->AttributesTakeDmg(Damage) < 0.f)
	{
		OnDeactivateEnemy_Implementation(true);
	}
	if (CharacterAttributes->GetVidaActual() <= CharacterAttributes->GetVidaMaxima() / 2)
	{
		PecadorAnimInstance->SetBehaviourState(EEnemyBehaviourState::EPWS_Injured);	
	}
	
	if (GameManager && HitInstigator->ActorHasTag("Player"))
	{
		GameManager->CurrentPlayer->OnDamageInflicted(Damage);
		if (EnemiesManager) EnemiesManager->OnEnemyBeginDamaged(this, Damage);
	}
	
}

void AEnemyTemplate::OnActivateEnemy_Implementation(FVector Position, FRotator Rotation)
{
	IEnemyInterface::OnActivateEnemy_Implementation(Position, Rotation);
	
	PecadorAnimInstance->SetBehaviourState(EEnemyBehaviourState::EEBS_Emerging);	
	TargetLocation = Position;
	StartLocation = Position - FVector(0.0f, 0.0f, 150.0f);
	SkeletalMeshComponent->SetComponentTickEnabled(true);
	SetActorRotation(Rotation);
	
	if (PortalFlipbookComponent)
	{
		PortalFlipbookComponent->SetWorldLocation(TargetLocation - FVector(0.0f, 0.0f, PortalZOffset));
		PortalFlipbookComponent->SetVisibility(true);
		PortalFlipbookComponent->SetPlaybackPosition(0.0f, false);
		PortalFlipbookComponent->Play();
	}
	
	SkeletalMeshComponent->Activate();
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
	SetActorEnableCollision(false);
	SetActorLocation(StartLocation, false, nullptr, ETeleportType::TeleportPhysics);
	SetActorHiddenInGame(false);
	
	GetWorldTimerManager().ClearTimer(EmergeTimerHandle);
	
	GetWorldTimerManager().SetTimer(
		EmergeTimerHandle, 
		this, 
		&AEnemyTemplate::UpdateEmergeMovement, 
		EmergeInterval, 
		true 
	);
}

void AEnemyTemplate::UpdateEmergeMovement()
{
	if (GetActorLocation().Z >= TargetLocation.Z)
	{
		bActorReady = true;
		GetWorldTimerManager().ClearTimer(EmergeTimerHandle);
		SetActorEnableCollision(true);
		if (PortalFlipbookComponent) PortalFlipbookComponent->SetVisibility(false);
		return;
	}
	
	if (PortalFlipbookComponent)
	{
		PortalFlipbookComponent->SetRelativeRotation(
			PortalFlipbookComponent->GetComponentRotation() + FRotator(0.0f, 2.0f, 0.f));
	}
	SetActorLocation(GetActorLocation() + FVector::UpVector * EmergeSpeedMultiplier);
}

void AEnemyTemplate::OnSlowEnemy_Implementation(float TimeAmount)
{
	IEnemyInterface::OnSlowEnemy_Implementation(TimeAmount);
	
	Velocidad = MaxVelocidad/3;
	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(TimerHandle_Slow,this,&AEnemyTemplate::OnSlowCD,TimeAmount,false);
}

void AEnemyTemplate::OnDeactivateEnemy_Implementation(bool bGiveExp)
{
	IEnemyInterface::OnDeactivateEnemy_Implementation(bGiveExp);

	SkeletalMeshComponent->SetComponentTickEnabled(false);
	SkeletalMeshComponent->Deactivate();
	
	SetActorLocation(FVector(0, 0, -50000.f)); 
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	if (EnemiesManager) EnemiesManager->ReturnEnemyToPool(this, bGiveExp);
}

void AEnemyTemplate::UpdateMovement(FVector NextPoint, float DeltaTime)
{
	if (!GameManager || !bActorReady) return;
	
	if (bHasAttackToken)
	{
		NextPoint = GameManager->CurrentPlayer->GetActorLocation();
	}
	
	FVector CurrentLoc = GetActorLocation();
	FVector Direction = (NextPoint - CurrentLoc).GetSafeNormal();
	Direction.Z = 0;
	
	if (LastFrameLocation == FVector(0.0f, 0.0f, 0.0f)) LastFrameLocation = CurrentLoc;

	float CurrentDistance = FVector::Dist(NextPoint, CurrentLoc);
	float Hysteresis = bHasAttackToken ? 50.f : 50.f;

	if (CurrentDistance > (StoppingDistance + Hysteresis))
	{
		bIsTargetReached = false; 
	}
	else if (CurrentDistance <= StoppingDistance)
	{
		bIsTargetReached = true;
	}

	if (!bIsTargetReached)
	{
        
		FVector NextLocation = CurrentLoc + (Direction * Velocidad * DeltaTime);
		FVector GroundNormal;
        
		AdjustLocationToGround(NextLocation, GroundNormal);
		AdjustRotationToGround(Direction, GroundNormal, DeltaTime);
		
		FHitResult Hit;
        
		SetActorLocation(NextLocation, true, &Hit);
		
		if (Hit.bBlockingHit)
		{
			FVector RemainingVelocity = NextLocation - Hit.ImpactPoint;
			FVector SlideVector = FVector::VectorPlaneProject(RemainingVelocity, Hit.Normal);

			AddActorWorldOffset(SlideVector, false);
		}
	}else
	{
		if (bHasAttackToken)
		{
			PecadorAnimInstance->OnAnimEnemyAttack();
			Velocidad = MaxVelocidad/3;
		}
	}
	
	//Animaciones
	float ActualSpeed = (CurrentLoc - LastFrameLocation).Size() / DeltaTime;
	LastFrameLocation = CurrentLoc;
	
	if (ActualSpeed <= ANIM_MOVE_THRESHOLD)
	{
		AnimHysteresis += 0.1f;
		
		if (AnimHysteresis >= 1.f)
		{
			PecadorAnimInstance->SetBehaviourState(EEnemyBehaviourState::EEBS_Idle);
			FVector DirectionToPlayer = (GameManager->CurrentPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			DirectionToPlayer.Z = 0;
    
			FRotator NewRotation = DirectionToPlayer.Rotation();
			SetActorRotation(NewRotation);
		}
	}else if (ActualSpeed > ANIM_MOVE_THRESHOLD)
	{
		AnimHysteresis = 0.f;
		if (CharacterAttributes->GetVidaActual() <= CharacterAttributes->GetVidaMaxima() / 2)
		{
			PecadorAnimInstance->SetBehaviourState(EEnemyBehaviourState::EPWS_Injured);	
		}
		else
		{
			PecadorAnimInstance->SetBehaviourState(EEnemyBehaviourState::EEBS_Run);
		}
	}
	
	
	if(GetActorLocation().Z > MAX_ALTITUDE)
	{
		LOG("Un enemigo se fue a la puta. Borrandolo...")
		Execute_OnDeactivateEnemy(this, false);
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
