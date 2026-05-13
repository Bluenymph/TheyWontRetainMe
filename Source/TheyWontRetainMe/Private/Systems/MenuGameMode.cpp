#include "Systems/MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/GameManager.h"

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
	
	if (!GameManager) return;
	
	TSubclassOf<UUserWidget> CurrentWidgetClass;
	
	if (GameManager->CurrentGameCycleMenu == 0 && StartGameMenu)
	{
		CurrentWidgetClass = StartGameMenu;
	}else if (GameManager->CurrentGameCycleMenu == 1 && DeathGameMenu)
	{
		CurrentWidgetClass = DeathGameMenu;
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR en MenuGameMode.cpp->BeginPlay : Fallan referencias."));
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

