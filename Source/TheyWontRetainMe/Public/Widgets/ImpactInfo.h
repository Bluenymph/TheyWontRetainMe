#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ImpactInfo.generated.h"

class UTextBlock;

/**
 * Clase para enseñar al jugador cuanto daño ha hecho. Se enseñara un texto con el numero de daño donde impacte la bala
 */
UCLASS()
class THEYWONTRETAINME_API UImpactInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetNumberDamage(float NewNumber);
	
	UFUNCTION(BlueprintCallable)
	void SetDistanceFontSize(float Distance);
	
protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DamageText;
};
