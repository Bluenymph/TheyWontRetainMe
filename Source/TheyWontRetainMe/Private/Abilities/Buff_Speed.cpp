#include "Abilities/Buff_Speed.h"
#include "Characters/PlayerTemplate.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBuff_Speed::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	APlayerTemplate* PlayerTemplate = static_cast<APlayerTemplate*>(InOwner);
	
	if (PlayerTemplate && PlayerTemplate->GetCharacterMovement())
	{
		PlayerTemplate->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(
			PlayerTemplate->GetCharacterMovement()->MaxWalkSpeed, 
			MAX_SPEED, 
			IncrementoPorcentual);
	}
}
