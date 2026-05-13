#include "Systems/ModoJuego.h"
#include "Systems/BulletPoolSubsystem.h"
#include "Systems/EnemiesManager.h"
#include "IAnimationBudgetAllocator.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/AbilitiesManager.h"

void AModoJuego::BeginPlay()
{
	Super::BeginPlay();
	
	IAnimationBudgetAllocator::Get(GetWorld())->SetEnabled(true);
	BulletSubsystem = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
	EnemiesManager = GetWorld()->GetSubsystem<UEnemiesManager>();
	
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("a.Budget.BudgetMs 5.0"));
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("a.Budget.Enabled 1"));

	if (SkillsDataBase)
	{
		UAbilitiesManager* Manager = GetWorld()->GetSubsystem<UAbilitiesManager>();
		if (Manager) Manager->LoadAllAbilities(SkillsDataBase);
	}
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		// Configura el modo para que ignore la interfaz y escuche solo al juego
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);

		// Oculta el cursor del ratón en la pantalla
		PC->bShowMouseCursor = false;
	}
	PrewarmPools();
}

void AModoJuego::PrewarmPools()
{
	if (!BulletSubsystem || !EnemiesManager || !GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Error en ModoJuego.cpp -> PrewarmPools(): Faltan referencias."));
		return;
	}
	
	for (int i = 0; i < BulletsToLoad.Num(); ++i)
	{
		BulletSubsystem->PrewarmPool(BulletsToLoad[i],200);
	}
	for (int i = 0; i < EnemiesToLoad.Num(); ++i)
	{
		EnemiesManager->PrewarmEnemyPool(EnemiesToLoad[i], 500);
		EnemiesManager->EnemiesToSpawn = EnemiesToLoad;
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginSpawnEnemies,this,&AModoJuego::SpawnEnemies,1.f,false);
}

void AModoJuego::SpawnEnemies()
{
	if (EnemiesToLoad.Num() > 0) EnemiesManager->BeginManageEnemiesLoop();
}
