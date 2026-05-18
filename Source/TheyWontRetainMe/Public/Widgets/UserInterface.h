#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserInterface.generated.h"

class UImage;
class UProgressBar;
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
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExpText;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExpBar;
	
	UPROPERTY(meta = (BindWidget))
	UImage* JumpImage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* JumpImageTextureActive;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* JumpImageTextureUnactive;
	
public:
	UFUNCTION()
	void SetHealth(const float NewHealth, const float MaxHealth);
	
	UFUNCTION()
	void SetAmmo(const float NewAmmo, const float MaxAmmo);
	
	UFUNCTION()
	void SetExp(const float NewExp, const float MaxExp);
	
	UFUNCTION()
	void SetJumpActive(const bool NewJumpActive);
};
