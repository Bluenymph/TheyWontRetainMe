#include "AnimInstances/PlayerAnimInstance.h"
#include "LogMacros.h"
#include "Characters/PlayerTemplate.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (PlayerCharacter)
	{
		CurrentMovementState = PlayerCharacter->GetPlayerMovementState();
		CurrentActionState = PlayerCharacter->GetPlayerActionState();
		CurrentInputMode = PlayerCharacter->GetPlayerInputMode();
		//Debug para mostrar los estados de animacion
		//LOG("Cambiando estado a: %s", *StaticEnum<EPlayerMovementState>()->GetNameStringByValue((int64)CurrentMovementState));
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<APlayerTemplate>(TryGetPawnOwner());
	}
}
