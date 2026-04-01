#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/HiteableInterface.h"
#include "EnemyTemplate.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class THEYWONTRETAINME_API AEnemyTemplate : public APawn, public IHiteableInterface
{
	GENERATED_BODY()

public:
	AEnemyTemplate();
	
	void OnHitReceived_Implementation(float Damage) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float VidaMaxima = 100.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float VidaActual = 100.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float Velocidad = 100.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float Danio = 30.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Comportamiento")
	float DistanciaAtaque = 3.f;
	
private:
	const float MAX_VELOCITY = 1000.f;
};
