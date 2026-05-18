#include "Abilities/Ability_Thunder.h"
#include "LogMacros.h"
#include "Characters/EnemyTemplate.h"
#include "Interfaces/AbilityVisualInterface.h"
#include "Interfaces/HiteableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Systems/GameManager.h"

void UAbility_Thunder::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	AbilityOwner = InOwner;
}

void UAbility_Thunder::OnVisualOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Super::OnVisualOverlap(OverlappedActor, OtherActor);
	
	if (OverlappedActor && OtherActor && OtherActor->Implements<UEnemyInterface>())
	{
		UGameManager* GameManager = OverlappedActor->GetGameInstance()->GetSubsystem<UGameManager>();
		GameManager->ShowImpacNumber(OverlappedActor->GetActorLocation(),OtherActor,Danio);
		IHiteableInterface::Execute_OnHitReceived(OtherActor, Danio, nullptr);
	}
}

void UAbility_Thunder::TrySpawnAbility_Implementation(FVector Position)
{
	IRandomImpactAbility::TrySpawnAbility_Implementation(Position);
	
	bool bSuccess = FMath::RandRange(1, 100) <= Probabilidad;
	
	if (bSuccess)
	{
		FVector SpawnLocation = Position;
		AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(VisualActorClass, FTransform(FRotator::ZeroRotator, SpawnLocation));

		if (NewActor)
		{
			if (NewActor->GetClass()->ImplementsInterface(UAbilityVisualInterface::StaticClass()))
			{
				IAbilityVisualInterface::Execute_SetParentAbility(NewActor, this);
			}
			NewActor->FinishSpawning(FTransform(FRotator::ZeroRotator, SpawnLocation));
			VisualActor = NewActor;
			VisualActor->UpdateOverlaps(true);
			
			if (GetWorld() && VisualActor)
			{
				UGameManager* GameManager = VisualActor->GetGameInstance()->GetSubsystem<UGameManager>();
				int32 Current = FCString::Atoi(*GameManager->GameStatistics.ThundersNum);
				Current++;
				GameManager->GameStatistics.ThundersNum = FString::FromInt(Current);
			}
			
			//SONIDOS
			if (!MetaSoundPlantilla || !SonidoTrueno) return;
	
			UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAtLocation(
				GetWorld(), 
				MetaSoundPlantilla, 
				VisualActor->GetActorLocation()
			);
    
			if (AudioComp)
			{
				AudioComp->SetWaveParameter(FName("SoundEffect"), SonidoTrueno);
				AudioComp->Play();
			}
		}
	}
}
