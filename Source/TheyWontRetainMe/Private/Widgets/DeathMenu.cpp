#include "Widgets/DeathMenu.h"

#include "LogMacros.h"
#include "Components/RichTextBlock.h" 
#include "Kismet/GameplayStatics.h"
#include "Systems/GameManager.h"

void UDeathMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
	GenerateStatistics();
}

void UDeathMenu::GoToStartMenu()
{
	if (!GetWorld()) return;
	
	GameManager->CurrentGameCycleMenu = 0;
	RemoveFromParent();
	const FName LevelName = FName("MainMenu");
	UGameplayStatics::OpenLevel(this, LevelName, true);
}

void UDeathMenu::GoToMainLevel()
{
	if (!GetWorld()) return;
	
	GameManager->CurrentGameCycleMenu = 0;
	RemoveFromParent();
	const FName LevelName = FName("BlockOut");
	UGameplayStatics::OpenLevel(this, LevelName, true);
}

void UDeathMenu::GenerateStatistics()
{
	if (!GameManager) return;
	
	FGameStatistics Estadisticas = GameManager->GameStatistics;
	FString StatText = "";
	
	//Danio infligido
	StatText += FString::Printf(TEXT("<Gods>Daño inflingido: </><Rojo>%s</>\n"),*Estadisticas.PlayerDmgInflinged);
	
	//Danio recibido
	StatText += FString::Printf(TEXT("<Gods>Daño recibido: </><Rojo>%s</>\n"),*Estadisticas.PlayerDmgReceived);
	
	//Curacion
	StatText += FString::Printf(TEXT("<Gods>Autocuracion: </><Rojo>%s</>\n"),*Estadisticas.PlayerAutoHeal);
	
	StatisticsBox->SetText(FText::FromString(*StatText));
	
}
