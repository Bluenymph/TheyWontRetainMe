#include "Systems/ModoJuego.h"
#include "Systems/BulletPoolSubsystem.h"
#include "Systems/EnemiesManager.h"
#include "IAnimationBudgetAllocator.h"
#include "Kismet/KismetSystemLibrary.h"

void AModoJuego::BeginPlay()
{
	Super::BeginPlay();
	IAnimationBudgetAllocator::Get(GetWorld())->SetEnabled(true);
	UBulletPoolSubsystem* BulletSubsystem = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
	UEnemiesManager* EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("a.Budget.BudgetMs 5.0"));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("a.Budget.Enabled 1"));

	if (BulletSubsystem)
	{
		for (int i = 0; i < BulletsToLoad.Num(); ++i)
		{
			BulletSubsystem->PrewarmPool(BulletsToLoad[i],200);
			EnemiesManager->PrewarmEnemyPool(EnemiesToLoad[i], 500);
			EnemiesManager->EnemiesToSpawn = EnemiesToLoad;
			EnemiesManager->BeginManageEnemiesLoop();
		}
	}
}
