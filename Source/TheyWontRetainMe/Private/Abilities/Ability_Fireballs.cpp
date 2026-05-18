#include "Abilities/Ability_Fireballs.h"
#include "LogMacros.h"
#include "Interfaces/AbilityVisualInterface.h"
#include "Interfaces/EnemyInterface.h"
#include "Components/AudioComponent.h"
#include "Interfaces/HiteableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/GameManager.h"

void UAbility_Fireballs::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	if (!InOwner || !VisualActorClass) return;
	
	AbilityOwner = InOwner;

	FVector SpawnLocation = AbilityOwner->GetActorLocation() + FVector(0, 0, 100);
	VisualActor = GetWorld()->SpawnActor<AActor>(VisualActorClass, SpawnLocation, FRotator::ZeroRotator);
	
	if (VisualActor && VisualActor->GetClass()->ImplementsInterface(UAbilityVisualInterface::StaticClass()))
	{
		IAbilityVisualInterface::Execute_SetParentAbility(VisualActor, this);
	}
	
	//SONIDOS
	if (!MetaSoundPlantilla || !SonidoFuego) return;
	
	UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(), 
		MetaSoundPlantilla, 
		VisualActor->GetActorLocation()
	);
    
	if (AudioComp)
	{
		AudioComp->SetWaveParameter(FName("FireEffect"), SonidoFuego);
		AudioComp->Play();
	}
}

void UAbility_Fireballs::TickAbility(float DeltaTime)
{
	Super::TickAbility(DeltaTime);
	
	if (VisualActor && AbilityOwner)
	{
		FVector TargetLoc = AbilityOwner->GetActorLocation();
		FVector NewLoc = FMath::VInterpTo(VisualActor->GetActorLocation(), TargetLoc, DeltaTime, 10.0f);
		VisualActor->SetActorLocation(NewLoc);

		FRotator CurrentRot = VisualActor->GetActorRotation();
		CurrentRot.Yaw += DeltaTime * 180.0f;
		VisualActor->SetActorRotation(CurrentRot);
	}
}

void UAbility_Fireballs::OnVisualOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnVisualOverlap(OverlappedActor, OtherActor);
	
	if (OtherActor->Implements<UEnemyInterface>())
	{
		UGameManager* GameManager = OverlappedActor->GetGameInstance()->GetSubsystem<UGameManager>();
		GameManager->ShowImpacNumber(OverlappedActor->GetActorLocation(),OtherActor,Damage);
		IHiteableInterface::Execute_OnHitReceived(OtherActor, Damage, nullptr);
	}
}
