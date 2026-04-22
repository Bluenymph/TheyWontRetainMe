#include "Systems/AbilitiesManager.h"
#include "LogMacros.h"
#include "DataAsset/AbilityData.h"
#include "Abilities/BaseAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/BaseAbility.h"

void UAbilitiesManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_TickAbilities, this, &UAbilitiesManager::TickAbilities,0.02f,true);
}

void UAbilitiesManager::NextLevel()
{
	LOG("Subida de nivel!")
}

void UAbilitiesManager::TickAbilities()
{
	for (int i = ActiveAbilities.Num()-1; i >= 0; i--)
	{
		if (ActiveAbilities.IsValidIndex(i) && ActiveAbilities[i])
		{
			ActiveAbilities[i]->TickAbility(GetWorld()->GetDeltaSeconds());
		}
	}
}

void UAbilitiesManager::AddAbilityFromData(const UAbilityData* AbilityData)
{
	if (!AbilityData) return;
	if (AbilityData->bIsUnique && FindAbility(AbilityData) > -1) return;
	
	LOG("Aniadiendo habilidad")
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	UBaseAbility* NewAbility = NewObject<UBaseAbility>(this, AbilityData->AbilityClass);
	NewAbility->AbilityName = AbilityData->AbilityName;
	
	
	if (NewAbility)
	{
		ActiveAbilities.Add(NewAbility);

		NewAbility->ActivateAbility(PlayerPawn);
	}
}

int UAbilitiesManager::FindAbility(const UAbilityData* AbilityData)
{
	for (int i = ActiveAbilities.Num()-1; i >= 0; i--)
	{
		if (ActiveAbilities[i]->AbilityName.EqualTo(AbilityData->AbilityName)) return i;
	}
	return -1;
}
