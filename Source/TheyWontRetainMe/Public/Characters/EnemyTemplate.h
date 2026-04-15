#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/HiteableInterface.h"
#include "EnemyTemplate.generated.h"

class UPecadorAnimInstance;
class UFloatingPawnMovement;
class UCapsuleComponent;
class UStaticMeshComponent;
class UCharacterAttributes;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyAttack, bool, bIsAttack);

UCLASS()
class THEYWONTRETAINME_API AEnemyTemplate : public APawn, public IHiteableInterface, public  IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyTemplate();
	
	void OnHitReceived_Implementation(float Damage) override;
	void OnActivateEnemy_Implementation(FVector Position, FRotator Rotation) override;
	void OnDeactivateEnemy_Implementation() override;

	void UpdateMovement(APawn* Player, float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FOnEnemyAttack OnEnemyAttack;

	FORCEINLINE UCharacterAttributes* GetCharacterAttributes() { return CharacterAttributes; }

protected:
	UFUNCTION()
	void AdjustLocationToGround(FVector& OutLocation, FVector& OutNormal);

	UFUNCTION()
	void AdjustRotationToGround(const FVector& Direction, const FVector& GroundNormal, float DeltaTime);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Referencias")
	UCharacterAttributes* CharacterAttributes;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float Velocidad = 100.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float DistanciaAtaque = 3.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Atributos")
	float StoppingDistance = 150.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Atributos")
	float Speed = 400.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;
	
private:
	const float MAX_VELOCITY = 1000.f;
	float ZOffset = 10.f; 
};
