#include "Systems/BulletPoolSubsystem.h"

#include "LogMacros.h"
#include "Actors/BulletTemplate.h"

void UBulletPoolSubsystem::PrewarmPool(TSubclassOf<ABulletTemplate> BulletClass, int32 Amount)
{
	if (!BulletClass) return;
	
	FBulletPool& Pool = PoolMap.FindOrAdd(BulletClass);

	for (int32 i = 0; i < Amount; ++i)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
		ABulletTemplate* NewBullet = GetWorld()->SpawnActor<ABulletTemplate>(BulletClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);
        
		if (NewBullet)
		{
			//La apagamos inmediatamente y la guardamos para usarla despues
			NewBullet->SetActorHiddenInGame(true);
			NewBullet->SetActorEnableCollision(false);
			Pool.InactiveBullets.Add(NewBullet);
		}
	}
}

ABulletTemplate* UBulletPoolSubsystem::GetBulletFromPool(TSubclassOf<ABulletTemplate> BulletClass, FVector Location, FRotator Rotation)
{
	if (!BulletClass) return nullptr;

	FBulletPool& Pool = PoolMap.FindOrAdd(BulletClass);
	ABulletTemplate* BulletToUse = nullptr;

	//Hay alguna bala libre en la piscina?
	if (Pool.InactiveBullets.Num() > 0)
	{
		BulletToUse = Pool.InactiveBullets.Pop();
	}
	else
	{
		//Si no hay spawneamos una nueva, esto no deberia pasar, idealmente...
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		BulletToUse = GetWorld()->SpawnActor<ABulletTemplate>(BulletClass, Location, Rotation, SpawnParams);
		LOG("Spawneando...")
	}

	if (BulletToUse)
	{
		//La movemos a su sitio y la activamos mediante la Interfaz
		BulletToUse->SetActorLocationAndRotation(Location, Rotation);
		
		//Llamamos a la función de la interfaz para activarla
		IBulletInterface::Execute_OnActivateBullet(BulletToUse, BulletToUse->GetActorForwardVector(), 1.f,5000.f);
		LOG("Activando la bala")
	}

	LOG("Sacando de la pool")
	return BulletToUse;
}

void UBulletPoolSubsystem::ReturnBulletToPool(ABulletTemplate* Bullet)
{
	if (!Bullet) return;
	LOG("Devolviendo a la pool...");

	//Desactivamos la bala para que no de por culo
	IBulletInterface::Execute_OnDeactivateBullet(Bullet);

	//Guardamos la bala en la piscina
	FBulletPool& Pool = PoolMap.FindOrAdd(Bullet->GetClass());
	Pool.InactiveBullets.Add(Bullet);
}