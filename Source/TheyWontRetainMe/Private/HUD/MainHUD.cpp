#include "HUD/MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/CharacterAttributes.h"
#include "Systems/GameManager.h"
#include "Widgets/UserInterface.h"


void AMainHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (UIClass)
	{
		APlayerController* PC = GetOwningPlayerController();
		if (PC && UIClass)
		{
			CurrentUI = CreateWidget<UUserWidget>(PC, UIClass);
			if (CurrentUI)
			{
				CurrentUI->AddToViewport();
			}
		}

		UserInterface = Cast<UUserInterface>(CurrentUI);
	}
	
	GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
}

void AMainHUD::UpdateUIInfo(UCharacterAttributes* Atributos)
{
	if (UserInterface)
	{
		UserInterface->SetHealth(Atributos->GetVidaActual(), Atributos->GetVidaMaxima());
		UserInterface->SetAmmo(Atributos->GetBalasActuales(), Atributos->GetMaxBalas());
		UserInterface->SetExp(GameManager->GetCurrentEXP(),GameManager->GetNextLevelThreshold());
	}
}

void AMainHUD::UpdateJumpInfo(const bool bActive)
{
	UserInterface->SetJumpActive(bActive);
}
