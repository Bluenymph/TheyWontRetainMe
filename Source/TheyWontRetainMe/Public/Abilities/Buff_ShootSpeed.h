#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Buff_ShootSpeed.generated.h"

UCLASS()
class THEYWONTRETAINME_API UBuff_ShootSpeed : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(AActor* InOwner) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float CantidadVelocidad = 0.1f;
};
