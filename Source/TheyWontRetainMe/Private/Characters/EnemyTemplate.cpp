#include "Characters/EnemyTemplate.h"

#include "DetailLayoutBuilder.h"
#include "LogMacros.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterAttributes.h"
#include "Components/StaticMeshComponent.h"

AEnemyTemplate::AEnemyTemplate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CapsuleComponent);

	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributes>("Atributos");
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
}
