#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/PlayerStates.h"
#include "PlayerAnimInstance.generated.h"

class APlayerTemplate;
class UAnimMontage;

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
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsFiring;

	UFUNCTION()
	void ActualizarEstadoDisparo(bool bNuevoEstado);
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* IzquierdaFuegoMontage;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DerechaFuegoMontage;
	
	UFUNCTION()
	void AnimNotify_FinDeAnimacion();
	
	UFUNCTION()
	void AnimNotify_FinDeEsquive();
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	int ManoDisparo = 0; //0 izquierda, 1 derecha
	
	UPROPERTY(BlueprintReadOnly)
	EPlayerMovementState CurrentMovementState;

	UPROPERTY(BlueprintReadOnly)
	EPlayerActionState CurrentActionState;

private:
	UPROPERTY()
	APlayerTemplate* PlayerCharacter;
	
	UFUNCTION()
	void Disparar();//Si, lo hago aqui. ¿Que pasa wacho?
};
