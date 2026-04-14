#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UserInterface.generated.h"

class UTextBlock;

/**
 *  El Widget donde se maneja la informacion HUD del juego.
 */
UCLASS()
class THEYWONTRETAINME_API UUserInterface : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;
	
public:
	FORCEINLINE void SetAmmo(const FText NewAmmo) { AmmoText->SetText(NewAmmo); }
	FORCEINLINE void SetHealth(const FText NewHealth) { HealthText->SetText(NewHealth); }
};
