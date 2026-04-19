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
	void OnAnimEnemyAttack();
	
	UFUNCTION(BlueprintCallable)
	void OnAnimEnemyAttacked(float RemainHealth);

	virtual void NativeInitializeAnimation() override;
	
	FORCEINLINE EEnemyBehaviourState GetBehaviourState() const { return BehaviourState; }
	FORCEINLINE void SetBehaviourState(EEnemyBehaviourState NewState) { BehaviourState = NewState; }

	
protected:
	UPROPERTY(BlueprintReadOnly)
	AEnemyTemplate* EnemyTemplate;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Animation")
	UAnimMontage* NormalAttackMontage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PecadorAnimInstance")
	EEnemyBehaviourState BehaviourState = EEnemyBehaviourState::EEBS_Idle;
	
	UFUNCTION()
	void AnimNotify_AttackEnded();
};
