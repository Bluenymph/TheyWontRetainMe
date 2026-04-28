#include "Actors/TurretActor.h"
#include "LogMacros.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"
#include "Kismet/KismetMathLibrary.h"


ATurretActor::ATurretActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	SetRootComponent(CapsuleCollision);
	
	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScopeMesh");
	ScopeMesh->SetupAttachment(RootComponent);
	
	BodyTurret = CreateDefaultSubobject<UStaticMeshComponent>("BodyTurret");
	BodyTurret->SetupAttachment(RootComponent);
}

void ATurretActor::RotateTurret(FVector NewPosition)
{
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewPosition);
	LookAt.Yaw -= 90.0f;
	FQuat RelQuat = GetActorTransform().InverseTransformRotation(LookAt.Quaternion());
	ScopeMesh->SetRelativeRotation(RelQuat.Rotator());
}

void ATurretActor::BeginPlay()
{
	Super::BeginPlay();
	
}


