#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

class UUserWidget;
class UGameManager;

/**
 * Este game mode es para las escenas donde solo aparecen los menus del juego
 */
UCLASS()
class THEYWONTRETAINME_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> StartGameMenu;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> DeathGameMenu;
	
private:
	UPROPERTY()
	UGameManager* GameManager;
	
};
