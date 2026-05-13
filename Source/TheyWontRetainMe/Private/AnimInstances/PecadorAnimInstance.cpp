#include "AnimInstances/PecadorAnimInstance.h"
#include "LogMacros.h"
#include "Animation/AnimMontage.h"
#include "Characters/EnemyTemplate.h"
#include "Components/BoxComponent.h"
#include "Systems/EnemiesManager.h"


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
void UPecadorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!EnemyTemplate)
	{
		EnemyTemplate = Cast<AEnemyTemplate>(TryGetPawnOwner());
	}
	
	RandomIdle = GetRandomIdle();
}

int32 UPecadorAnimInstance::GetRandomIdle()
{
	float Prob = FMath::FRandRange(0.f, 100.f);
	int32 Resultado;

	if (Prob <= 40.f)
	{
		Resultado = 0;
	}
	else if (Prob <= 80.f) 
	{
		Resultado = 1;
	}
	else                   
	{
		Resultado = 2;
	}
	
	return Resultado;
}

void UPecadorAnimInstance::AnimNotify_AttackEnded()
{
	if (EnemyTemplate)
	{
		EnemyTemplate->GetHitComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EnemyTemplate->bHasAttackToken = false;
		EnemyTemplate->OnSlowCD();
		UEnemiesManager* EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
		if (EnemiesManager) EnemiesManager->EnemiesAttackTokens++;
	}
}

void UPecadorAnimInstance::AnimNotify_FinDeTaunt()
{
	RandomIdle = GetRandomIdle();
}
