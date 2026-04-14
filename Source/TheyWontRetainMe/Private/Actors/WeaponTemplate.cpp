#include "Actors/WeaponTemplate.h"
#include "LogMacros.h"

AWeaponTemplate::AWeaponTemplate()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponTemplate::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponTemplate::Fire()
{
	LOG("BUUM");
}

