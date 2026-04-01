#include "Characters/EnemyTemplate.h"
#include "LogMacros.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

AEnemyTemplate::AEnemyTemplate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CapsuleComponent);
}

void AEnemyTemplate::OnHitReceived_Implementation(float Damage)
{
	IHiteableInterface::OnHitReceived_Implementation(Damage);
	LOG("El enemigo %s ha recibido: %f de daño",*GetName(),Damage);
	LOG("Le queda: %f de vida.", VidaActual);
	
	VidaActual -= Damage;
	if (VidaActual < 0) LOG("El enemigo %s murio.", *GetName())
}

void AEnemyTemplate::BeginPlay()
{
	Super::BeginPlay();
	
}
