#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/PlayerStates.h"
#include "PlayerAnimInstance.generated.h"

class APlayerTemplate;

/**
 *  Clase que controla las animaciones del player
 */
UCLASS()
class THEYWONTRETAINME_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPlayerMovementState CurrentMovementState;

	UPROPERTY(BlueprintReadOnly, Category = "Action")
	EPlayerActionState CurrentActionState;

private:
	UPROPERTY()
	APlayerTemplate* PlayerCharacter;
};
