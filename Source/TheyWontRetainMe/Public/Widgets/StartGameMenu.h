#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartGameMenu.generated.h"

class UButton;
/**
 * El menu de inicio del juego
 */
UCLASS()
class THEYWONTRETAINME_API UStartGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> OptionsMenuClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* btn_Inicio;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* btn_Opciones;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* btn_Salir;
	
	UFUNCTION(BlueprintCallable)
	void LoadMainScene();
	
	UFUNCTION(BlueprintCallable)
	void SpawnOptions();
	
	UFUNCTION()
	void DespawnOptions();
	
};
