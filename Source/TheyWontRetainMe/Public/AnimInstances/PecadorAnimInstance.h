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

	virtual void NativeInitializeAnimation() override;
	
	FORCEINLINE EEnemyBehaviourState GetBehaviourState() const { return BehaviourState; }
	FORCEINLINE void SetBehaviourState(EEnemyBehaviourState NewState) { if (NewState != BehaviourState) BehaviourState = NewState; }

	
protected:
	UPROPERTY(BlueprintReadOnly)
	AEnemyTemplate* EnemyTemplate;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Animation")
	UAnimMontage* NormalAttackMontage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PecadorAnimInstance")
	EEnemyBehaviourState BehaviourState = EEnemyBehaviourState::EEBS_Idle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PecadorAnimInstance")
	int32 RandomIdle = 0;
	
	UFUNCTION()
	int32 GetRandomIdle();
	
	UFUNCTION()
	void AnimNotify_AttackEnded();
	
	UFUNCTION()
	void AnimNotify_FinDeTaunt();
};
