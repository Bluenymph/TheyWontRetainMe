#include "Widgets/UserInterface.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UUserInterface::SetHealth(const float NewHealth, const float MaxHealth)
{
	FString Result = FString::Printf(TEXT("%.0f/%.0f"),NewHealth,MaxHealth);
	
	HealthText->SetText(FText::FromString(Result));
	HealthBar->SetPercent(NewHealth/MaxHealth);
}

void UUserInterface::SetAmmo(const float NewAmmo, const float MaxAmmo)
{
	FString Result = FString::Printf(TEXT("Balas:%.0f/%.0f"),NewAmmo,MaxAmmo);
	
	AmmoText->SetText(FText::FromString(Result));
}

void UUserInterface::SetExp(const float NewExp, const float MaxExp)
{
	FString Result = FString::Printf(TEXT("%.0f/%.0f"),NewExp,MaxExp);
	
	ExpText->SetText(FText::FromString(Result));
	ExpBar->SetPercent(NewExp/MaxExp);
}

void UUserInterface::SetJumpActive(const bool NewJumpActive)
{
	if (NewJumpActive)
	{
		JumpImage->SetBrushFromTexture(JumpImageTextureActive);
	}else
	{
		JumpImage->SetBrushFromTexture(JumpImageTextureUnactive);
	}
}
