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
	
	GameManager->ResetExp();
	GameManager->ResetStatistics();
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
	
	//Tiempo juego
	StatText += FString::Printf(TEXT("<Gods>Tiempo con vida: </><Rojo>%ss</>\n"),*Estadisticas.SecondsAlive);
	
	//Niveles
	StatText += FString::Printf(TEXT("<Gods>Nivel: </><Rojo>%s</>\n"),*Estadisticas.PlayerLevel);
	
	//Enemigos matados
	StatText += FString::Printf(TEXT("<Gods>Enemigos papeados: </><Rojo>%s</>\n"),*Estadisticas.EnemiesKilled);
	
	//Danio infligido
	StatText += FString::Printf(TEXT("<Gods>Daño inflingido: </><Rojo>%s</>\n"),*Estadisticas.PlayerDmgInflinged);
	
	//Danio recibido
	StatText += FString::Printf(TEXT("<Gods>Daño recibido: </><Rojo>%s</>\n"),*Estadisticas.PlayerDmgReceived);
	
	//Curacion
	StatText += FString::Printf(TEXT("<Gods>Autocuracion: </><Rojo>%s</>\n"),*Estadisticas.PlayerAutoHeal);
	
	//Dashes
	if (Estadisticas.PlayerDashes != "0") 
		StatText += FString::Printf(TEXT("<Gods>Esquivas: </><Rojo>%s</>\n"),*Estadisticas.PlayerDashes);
	
	//Truenos
	if (Estadisticas.ThundersNum != "0") 
		StatText += FString::Printf(TEXT("<Gods>Rayos invocados: </><Rojo>%s</>\n"),*Estadisticas.ThundersNum);
	
	//Torretas
	if (Estadisticas.TurretsNum != "0") 
		StatText += FString::Printf(TEXT("<Gods>Torretas invocadas: </><Rojo>%s</>\n"),*Estadisticas.TurretsNum);
	
	
	StatisticsBox->SetText(FText::FromString(*StatText));
	
}
