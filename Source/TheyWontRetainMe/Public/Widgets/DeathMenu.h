#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathMenu.generated.h"

class UGameManager;
class URichTextBlock;
class UButton;

/**
 * Menu que se mostrara cuando el player pierda la partida.
 */
UCLASS()
class THEYWONTRETAINME_API UDeathMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* btn_Reintentar;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* btn_Salir;
	
	UPROPERTY(meta = (BindWidget))
	URichTextBlock* StatisticsBox;
	
	UFUNCTION(BlueprintCallable)
	void GoToStartMenu();
	
	UFUNCTION(BlueprintCallable)
	void GoToMainLevel();
	
private:
	UPROPERTY()
	UGameManager* GameManager;
	
	UFUNCTION()
	void GenerateStatistics();
	
};
