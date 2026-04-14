#include "Systems/ModoJuego.h"
#include "Systems/BulletPoolSubsystem.h"
#include "Systems/EnemiesManager.h"

void AModoJuego::BeginPlay()
{
	Super::BeginPlay();
	
	UBulletPoolSubsystem* BulletSubsystem = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
	UEnemiesManager* EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();

	if (BulletSubsystem)
	{
		for (int i = 0; i < BulletsToLoad.Num(); ++i)
		{
			BulletSubsystem->PrewarmPool(BulletsToLoad[i],200);
			EnemiesManager->PrewarmEnemyPool(EnemiesToLoad[i], 100);
			EnemiesManager->EnemiesToSpawn = EnemiesToLoad;
			EnemiesManager->BeginManageEnemiesLoop();
		}
	}
}
