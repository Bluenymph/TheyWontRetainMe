#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTemplate.generated.h"

class UNiagaraSystem;

UCLASS()
class THEYWONTRETAINME_API AWeaponTemplate : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponTemplate();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void Fire();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform BulletPoint;
	
	UPROPERTY(EditAnywhere, Category = "FX")
	UNiagaraSystem* TracerEffect;


};
