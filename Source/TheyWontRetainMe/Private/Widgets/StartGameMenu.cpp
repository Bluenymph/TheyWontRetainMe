#include "Widgets/StartGameMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/MenuPausa.h"

void UStartGameMenu::LoadMainScene()
{
	RemoveFromParent();

	const FName LevelName = FName("Blockout");
	UGameplayStatics::OpenLevel(this, LevelName, true);
}

void UStartGameMenu::SpawnOptions()
{
	UUserWidget* OptionsMenuWidget = CreateWidget<UUserWidget>(GetWorld(), OptionsMenuClass);

	if (OptionsMenuWidget)
	{
		OptionsMenuWidget->AddToViewport();
		btn_Inicio->SetIsEnabled(false);
		btn_Opciones->SetIsEnabled(false);
		btn_Salir->SetIsEnabled(false);
		if (UMenuPausa* Menu = static_cast<UMenuPausa*>(OptionsMenuWidget))
		{
				Menu->OnMenuPausaClosed.AddUniqueDynamic(this, &UStartGameMenu::DespawnOptions);
		}
	}
}

void UStartGameMenu::DespawnOptions()
{
	btn_Inicio->SetIsEnabled(true);
	btn_Opciones->SetIsEnabled(true);
	btn_Salir->SetIsEnabled(true);
}
