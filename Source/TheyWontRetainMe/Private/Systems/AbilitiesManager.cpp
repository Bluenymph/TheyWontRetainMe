#include "Systems/AbilitiesManager.h"
#include "LogMacros.h"
#include "DataAsset/AbilityData.h"
#include "Abilities/BaseAbility.h"
#include "Kismet/GameplayStatics.h"
#include "DataAsset/AbilitiesDataBase.h"
#include "Abilities/BaseAbility.h"
#include "Interfaces/RandomImpactAbility.h"

void UAbilitiesManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_TickAbilities, this, &UAbilitiesManager::TickAbilities,0.02f,true);
}

void UAbilitiesManager::LoadAllAbilities(UAbilitiesDataBase* DataBase)
{
	for (int i = DataBase->AllAbilities.Num()-1; i >= 0; i--)
	{
		AllAbilitiesAvailable.Add(DataBase->AllAbilities[i]);
	}
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

void UAbilitiesManager::AddAbilityFromData(UAbilityData* AbilityData)
{
	if (!AbilityData) return;
	if (AbilityData->bIsUnique && FindAbility(AbilityData) > -1) return;
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	UBaseAbility* NewAbility = NewObject<UBaseAbility>(this, AbilityData->AbilityClass);
	NewAbility->AbilityName = AbilityData->AbilityName;
	
	
	if (NewAbility)
	{
		ActiveAbilities.Add(NewAbility);

		NewAbility->ActivateAbility(PlayerPawn);
		
		if (AbilityData->Limite > 0)
		{
			int32& Stacks = AbilitiesStack.FindOrAdd(AbilityData);
			Stacks++;
		}
	}
}

void UAbilitiesManager::ImpactEffect_CallAbilities(FVector Position)
{
	for (int i = ActiveAbilities.Num()-1; i >= 0; i--)
	{
		if (ActiveAbilities.IsValidIndex(i) && ActiveAbilities[i] && 
			ActiveAbilities[i]->GetClass()->ImplementsInterface(URandomImpactAbility::StaticClass()))
		{
			IRandomImpactAbility::Execute_TrySpawnAbility(ActiveAbilities[i], Position);
		}
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

int UAbilitiesManager::FindAbilityByName(FText Name)
{
	for (int i = ActiveAbilities.Num()-1; i >= 0; i--)
	{
		if (ActiveAbilities[i]->AbilityName.EqualTo(Name)) return i;
	}
	return -1;
}
