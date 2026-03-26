#include "Systems/ModoJuego.h"
#include "Systems/BulletPoolSubsystem.h"

void AModoJuego::BeginPlay()
{
	Super::BeginPlay();
	
	UBulletPoolSubsystem* BulletSubsystem = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();

	if (BulletSubsystem)
	{
		for (int i = 0; i < BulletsToLoad.Num(); ++i)
		{
			BulletSubsystem->PrewarmPool(BulletsToLoad[i],200);
		}
	}
}
