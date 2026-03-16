#include "Characters/PlayerTemplate.h"

APlayerTemplate::APlayerTemplate()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerTemplate::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerTemplate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

