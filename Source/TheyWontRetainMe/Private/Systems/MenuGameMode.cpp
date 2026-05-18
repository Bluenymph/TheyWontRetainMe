#include "Systems/MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Systems/GameManager.h"

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
	
	if (!GameManager) return;
	
	TSubclassOf<UUserWidget> CurrentWidgetClass;
	USoundCue* SelectedMusic = nullptr;
	
	if (GameManager->CurrentGameCycleMenu == 0 && StartGameMenu)
	{
		CurrentWidgetClass = StartGameMenu;
		SelectedMusic = Musica_MenuInicio;
	}else if (GameManager->CurrentGameCycleMenu == 1 && DeathGameMenu)
	{
		CurrentWidgetClass = DeathGameMenu;
		SelectedMusic = Musica_MenuMuerte;
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR en MenuGameMode.cpp->BeginPlay : Fallan referencias."));
	}
	
	if (SelectedMusic)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SelectedMusic);
	}
	
	UUserWidget* CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), CurrentWidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

