#include "Widgets/MenuPausa.h"

#include "LogMacros.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuPausa::NativeConstruct()
{
	Super::NativeConstruct();
	
	BtnSalir->OnClicked.AddUniqueDynamic(this,&UMenuPausa::Salir);
	BtnAplicar->OnClicked.AddUniqueDynamic(this,&UMenuPausa::SaveAndApply);
	BtnSalirJuego->OnClicked.AddUniqueDynamic(this,&UMenuPausa::ExitGame);
}

void UMenuPausa::Salir()
{
	if (OnMenuPausaClosed.IsBound()) OnMenuPausaClosed.Broadcast();
	RemoveFromParent();
}

void UMenuPausa::SaveAndApply()
{
	LOG("Aplicando... SliderMouse: %f  y  ComboBoxTring: %s", SliderMouse->GetValue(), *ComboBoxResolucion->GetSelectedOption())
	
	Salir();
}

void UMenuPausa::ExitGame()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(), 
		GetWorld()->GetFirstPlayerController(), 
		EQuitPreference::Quit, 
		false // Ignorar el guardado automático de configuración de Unreal
	);
}
