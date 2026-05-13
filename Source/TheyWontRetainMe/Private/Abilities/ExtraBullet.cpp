#include "Abilities/ExtraBullet.h"

#include "Systems/AbilitiesManager.h"

void UExtraBullet::ActivateAbility(AActor* InOwner)
{
	Super::ActivateAbility(InOwner);
	
	UAbilitiesManager* AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	AbilitiesManager->SetExtraBulletsQuantity(AbilitiesManager->GetExtraBulletsQuantity() + CantidadAumentar);
}
