#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuPausa.generated.h"

class UComboBoxString;
class USlider;
class UButton;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuPausaClosed);

/**
 * Menu de pausa en el juego y también en opciones
 */
UCLASS()
class THEYWONTRETAINME_API UMenuPausa : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	FOnMenuPausaClosed OnMenuPausaClosed;
	
protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BtnAplicar;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BtnSalir;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BtnSalirJuego;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* SliderMouse;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ComboBoxResolucion;
	
	UFUNCTION(BlueprintCallable)
	void Salir();
	
	UFUNCTION(BlueprintCallable)
	void SaveAndApply();
	
	UFUNCTION(BlueprintCallable)
	void ExitGame();
};
