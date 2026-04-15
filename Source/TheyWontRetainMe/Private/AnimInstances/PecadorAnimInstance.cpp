#include "AnimInstances/PecadorAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Characters/EnemyTemplate.h"


void UPecadorAnimInstance::OnAnimPecadorAttack(bool Attack)
{
	if (NormalAttackMontage) Montage_Play(NormalAttackMontage, 1.0f);
}

void UPecadorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!EnemyTemplate)
	{
		EnemyTemplate = Cast<AEnemyTemplate>(TryGetPawnOwner());
		if (EnemyTemplate)
		{
			EnemyTemplate->OnEnemyAttack.AddDynamic(this, &UPecadorAnimInstance::OnAnimPecadorAttack);
		}
	}
}
