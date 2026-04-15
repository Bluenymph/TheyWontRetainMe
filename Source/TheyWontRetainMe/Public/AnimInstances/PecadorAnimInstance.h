#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/EnemiesStates.h"
#include "PecadorAnimInstance.generated.h"

class AEnemyTemplate;
/**
 * Clase para controlar las animaciones de los pecadores
 */
UCLASS()
class THEYWONTRETAINME_API UPecadorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void OnAnimPecadorAttack(bool Attack);

	virtual void NativeInitializeAnimation() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AEnemyTemplate* EnemyTemplate;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Animation")
	UAnimMontage* NormalAttackMontage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PecadorAnimInstance")
	EEnemyBehaviourState BehaviourState = EEnemyBehaviourState::EEBS_Idle;
};
