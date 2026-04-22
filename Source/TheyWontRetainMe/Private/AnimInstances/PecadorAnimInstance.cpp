#include "AnimInstances/PecadorAnimInstance.h"
#include "LogMacros.h"
#include "Animation/AnimMontage.h"
#include "Characters/EnemyTemplate.h"
#include "Components/BoxComponent.h"
#include "Components/CharacterAttributes.h"


void UPecadorAnimInstance::OnAnimEnemyAttack()
{
	if (NormalAttackMontage && !Montage_IsPlaying(NormalAttackMontage))
	{
		Montage_Play(NormalAttackMontage, 1.0f);
	}
	if (EnemyTemplate)
	{
		EnemyTemplate->GetHitComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UPecadorAnimInstance::OnAnimEnemyAttacked(float RemainHealth)
{
	if (RemainHealth <= EnemyTemplate->GetCharacterAttributes()->GetVidaMaxima() / 2)
	{
		BehaviourState = EEnemyBehaviourState::EPWS_Injured;
	}
}

void UPecadorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!EnemyTemplate)
	{
		EnemyTemplate = Cast<AEnemyTemplate>(TryGetPawnOwner());
	}
}

void UPecadorAnimInstance::AnimNotify_AttackEnded()
{
	if (EnemyTemplate)
	{
		EnemyTemplate->GetHitComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
