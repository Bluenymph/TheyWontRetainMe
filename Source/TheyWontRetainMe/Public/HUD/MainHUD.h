#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UGameManager;
class UCharacterAttributes;
class UUserInterface;
class UUserWidget;

/**
 * El HUD principal del juego. Donde se muestra la vida, municion y exp.
 */
UCLASS()
class THEYWONTRETAINME_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> UIClass;

	UPROPERTY()
	UUserWidget* CurrentUI;

	UPROPERTY()
	UUserInterface* UserInterface;

	UFUNCTION(BlueprintCallable)
	void UpdateUIInfo(UCharacterAttributes* Atributos);
	
	UFUNCTION(BlueprintCallable)
	void UpdateJumpInfo(const bool bActive);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	UGameManager* GameManager;
};
