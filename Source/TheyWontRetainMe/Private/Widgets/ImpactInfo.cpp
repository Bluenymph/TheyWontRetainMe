#include "Widgets/ImpactInfo.h"
#include "LogMacros.h"
#include "Components/TextBlock.h"

void UImpactInfo::SetNumberDamage(float NewNumber)
{
	DamageText->SetText(FText::AsNumber(NewNumber));
}

void UImpactInfo::SetDistanceFontSize(float Distance)
{
	FSlateFontInfo NewFont = DamageText->GetFont();
	FVector2D DistanciaRango(200.f, 700.f);
	FVector2D TamanoRango(60.f, 20.f);

	NewFont.Size = FMath::GetMappedRangeValueClamped(DistanciaRango, TamanoRango, Distance);

	DamageText->SetFont(NewFont);
}
