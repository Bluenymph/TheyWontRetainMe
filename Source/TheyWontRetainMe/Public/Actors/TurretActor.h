#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretActor.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;

UCLASS()
class THEYWONTRETAINME_API ATurretActor : public AActor
{
	GENERATED_BODY()

public:
	ATurretActor();
	
	UFUNCTION(BlueprintCallable)
	void RotateTurret(FVector NewPosition);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCapsuleComponent* CapsuleCollision;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* ScopeMesh;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* BodyTurret;

};
