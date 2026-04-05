#include "HUD/MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/CharacterAttributes.h"
#include "Widgets/UserInterface.h"


void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UIClass)
	{
		APlayerController* PC = GetOwningPlayerController();
		if (PC && UIClass)
		{
			CurrentUI = CreateWidget<UUserWidget>(PC, UIClass);
			if (CurrentUI)
			{
				CurrentUI->AddToViewport();
			}
		}

		UserInterface = Cast<UUserInterface>(CurrentUI);
	}
}

void AMainHUD::UpdateUIInfo(UCharacterAttributes* Atributos)
{
	if (UserInterface)
	{
		FText Health = FText::AsNumber(Atributos->GetVidaActual());
		FText MaxHealth = FText::AsNumber(Atributos->GetVidaMaxima());
		FText Ammo = FText::AsNumber(Atributos->GetBalasActuales());
		FText MaxAmmo = FText::AsNumber(Atributos->GetMaxBalas());
		
		FText HealthFinal = FText::Format(
			NSLOCTEXT("MiJuego", "AmmoKey", "Vida: {Actual} de {Total}"),
			Health,
			MaxHealth
		);
		
		FText AmmoFinal = FText::Format(
			NSLOCTEXT("MiJuego", "AmmoKey", "Munición: {Actual} de {Total}"),
			Ammo,
			MaxAmmo
		);
		
		UserInterface->SetHealth(HealthFinal);
		UserInterface->SetAmmo(AmmoFinal);
	}
}
