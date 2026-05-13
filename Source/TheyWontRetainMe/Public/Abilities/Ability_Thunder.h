#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_Thunder.generated.h"

class UGameManager;
class UEnemiesManager;
class AEnemyTemplate;
/**
 * Habilidad que invoca un rayo de manera aleatoria cuando le haces daño a un enemigo
 */
UCLASS()
class THEYWONTRETAINME_API UAbility_Thunder : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<AActor> VisualActorClass;
	
	virtual void ActivateAbility(AActor* InOwner) override;
	virtual void OnVisualOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
	
	UFUNCTION(BlueprintCallable)
	void TryInvokeThunder(AEnemyTemplate* EnemyTemplate, float Damage);
	
	FORCEINLINE void SetProbabilidad(const float NuevaProbabilidad) { this->Probabilidad = NuevaProbabilidad; }
	
	FORCEINLINE float GetProbabilidad() const { return Probabilidad; }
	
protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Probabilidad = 10; //de  0 a 100 porciento...
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Danio = 30.f;
	
private:
	UPROPERTY()
	UEnemiesManager* EnemiesManager;
	
};
