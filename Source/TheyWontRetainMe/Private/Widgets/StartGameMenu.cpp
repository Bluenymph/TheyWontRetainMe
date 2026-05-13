#include "Widgets/StartGameMenu.h"
#include "Kismet/GameplayStatics.h"

void UStartGameMenu::LoadMainScene()
{
	RemoveFromParent();

	const FName LevelName = FName("Blockout");
	UGameplayStatics::OpenLevel(this, LevelName, true);
}