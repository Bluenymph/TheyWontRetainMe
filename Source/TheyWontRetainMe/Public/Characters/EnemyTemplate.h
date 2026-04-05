#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/HiteableInterface.h"
#include "EnemyTemplate.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UCharacterAttributes;

UCLASS()
class THEYWONTRETAINME_API AEnemyTemplate : public APawn, public IHiteableInterface, public  IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyTemplate();
	
	void OnHitReceived_Implementation(float Damage) override;
	void OnActivateEnemy_Implementation(FVector Position, FRotator Rotation) override;
	void OnDeactivateEnemy_Implementation() override;

protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Atributos")
	UCharacterAttributes* CharacterAttributes;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float Velocidad = 100.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Comportamiento")
	float DistanciaAtaque = 3.f;
	
private:
	const float MAX_VELOCITY = 1000.f;
};
