#include "Widgets/LevelUp.h"
#include "LogMacros.h"
#include "Characters/PlayerTemplate.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "DataAsset/AbilityData.h"
#include "Systems/AbilitiesManager.h"
#include "Systems/GameManager.h"

void ULevelUp::InitLevelUp()
{
	if (!AbilitiesManager) 
		AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	
	
	if (!AbilitiesManager || AbilitiesManager->AllAbilitiesAvailable.Num() <= 0) return;
	
	TArray<UAbilityData*> SelectedAbilities = SelectThreeRngSkills(AbilitiesManager->AllAbilitiesAvailable);
	TArray<UUserWidget*> Botones;
	Botones.Add(WBP_SkillButton);
	Botones.Add(WBP_SkillButton_1);
	Botones.Add(WBP_SkillButton_2);
	
	for (int32 i = 0; i < SelectedAbilities.Num(); i++)
	{
		UWidget* Widget = Botones[i]->GetWidgetFromName(TEXT("SkillName"));
		UTextBlock* Texto = Cast<UTextBlock>(Widget);
		
		ButtonsBindedAbilities.Add(SelectedAbilities[i]);
		
		if (Texto) Texto->SetText(SelectedAbilities[i]->AbilityName);
		
		Widget = Botones[i]->GetWidgetFromName(TEXT("SkillImage"));
		
		UImage* Image = Cast<UImage>(Widget);
		Image->SetBrushFromTexture(SelectedAbilities[i]->Image);
		
		Widget = Botones[i]->GetWidgetFromName(TEXT("SkillDescription"));
		URichTextBlock* TextBox = Cast<URichTextBlock>(Widget);
		TextBox->SetText(SelectedAbilities[i]->Descripcion);
		
		Widget = Botones[i]->GetWidgetFromName(TEXT("SkillButton"));
		UButton* Button = Cast<UButton>(Widget);
		
		if (Button) Button->OnPressed.AddUniqueDynamic(this, &ULevelUp::OnSkillPressed);
	}
}

void ULevelUp::OnSkillPressed()
{
	UGameManager* GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
	
	if (!AbilitiesManager || !GameManager)
	{
		UE_LOG(LogTemp, Error, TEXT("No se encuentra las referencias adecuadas en LevelUp.cpp->OnSkillPressed()"))
		return;
	}
	
	if (UButton* btn = Cast<UButton>(WBP_SkillButton->GetWidgetFromName(TEXT("SkillButton"))))
	{
		if (btn->IsPressed())
		{
			LOG("Añadiendo habilidad: %s", *ButtonsBindedAbilities[0]->AbilityName.ToString())
			AbilitiesManager->AddAbilityFromData(ButtonsBindedAbilities[0]);
			
		}
	}
	
	if (UButton* btn = Cast<UButton>(WBP_SkillButton_1->GetWidgetFromName(TEXT("SkillButton"))))
	{
		if (btn->IsPressed())
		{
			LOG("Añadiendo habilidad: %s", *ButtonsBindedAbilities[1]->AbilityName.ToString())
			AbilitiesManager->AddAbilityFromData(ButtonsBindedAbilities[1]);
		}
	}
	
	if (UButton* btn = Cast<UButton>(WBP_SkillButton_2->GetWidgetFromName(TEXT("SkillButton"))))
	{
		if (btn->IsPressed())
		{
			LOG("Añadiendo habilidad: %s", *ButtonsBindedAbilities[2]->AbilityName.ToString())
			AbilitiesManager->AddAbilityFromData(ButtonsBindedAbilities[2]);
		}
	}
	
	ButtonsBindedAbilities.Empty();
	GameManager->CurrentPlayer->UnPause();
}

TArray<UAbilityData*> ULevelUp::SelectThreeRngSkills(TArray<UAbilityData*> AllAbilitiesAvailable)
{
	//Primero eliminamos imposibles
	TArray<UAbilityData*> Suitables;
	for (int i = AbilitiesManager->AllAbilitiesAvailable.Num() - 1; i >= 0; --i)
	{
		UAbilityData* AbilityData = AllAbilitiesAvailable[i];
		
		//Si es único y ya esta aplicado no
		if (AbilityData->bIsUnique && AbilitiesManager->FindAbility(AbilityData) > -1) continue;
		
		
		//Si no tenemos los requisitos no
		if (AbilityData->Requisitos.Num() > 0)
		{
			bool bAllFinded = true;
			for (int j = 0; j < AbilityData->Requisitos.Num(); j++)
			{
				if (AbilitiesManager->FindAbility(AbilityData->Requisitos[j]) < 0)
				{
					bAllFinded = false;
					break;
				}
			}
			if (!bAllFinded) continue;
		}
		
		//Si ya se ha aplicado el numero maximo de veces no
		int32* StackActual = AbilitiesManager->AbilitiesStack.Find(AbilityData);
		if (StackActual && *StackActual >= AbilityData->Limite) continue;
		
		Suitables.Add(AbilityData);
	}
	
	for (UAbilityData* AbilityData : Suitables) LOG("Habilidad posible: %s", *AbilityData->AbilityName.ToString())
	
	//Tiramos de ruleta
	TArray<UAbilityData*> Pool = Suitables;
	TArray<UAbilityData*> Seleccionados;

	for (int32 Iter = 0; Iter < 3 && Pool.Num() > 0; Iter++)
	{
		int32 SumaProbabilidades = 0;
		for (UAbilityData* Actor : Pool) SumaProbabilidades += Actor->Probabilidad;

		int32 RandomPoint = FMath::RandRange(1, SumaProbabilidades);
		int32 CuentaActual = 0;

		//Vamos a ver donde ha caido
		for (int32 i = 0; i < Pool.Num(); i++)
		{
			CuentaActual += Pool[i]->Probabilidad;
			if (RandomPoint <= CuentaActual)
			{
				Seleccionados.Add(Pool[i]);
				Pool.RemoveAt(i); //Lo quitamos para que no salga repetido
				break;
			}
		}
	}
	return Seleccionados;
}
