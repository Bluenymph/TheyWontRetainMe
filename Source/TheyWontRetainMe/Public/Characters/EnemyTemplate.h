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
class UBoxComponent;


UCLASS()
class THEYWONTRETAINME_API AEnemyTemplate : public APawn, public IHiteableInterface, public  IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyTemplate();
	virtual void BeginPlay() override;
	
	void OnHitReceived_Implementation(float Damage) override;
	void OnActivateEnemy_Implementation(FVector Position, FRotator Rotation) override;
	void OnDeactivateEnemy_Implementation() override;

	void UpdateMovement(APawn* Player, float DeltaTime);
	
	FORCEINLINE UCharacterAttributes* GetCharacterAttributes() { return CharacterAttributes; }
	FORCEINLINE void SetVelocidad(const float NuevaVelocidad) { Velocidad = NuevaVelocidad; }
	FORCEINLINE float GetVelocidad() const { return Velocidad; }
	FORCEINLINE float GetMaxVelocidad() const { return MaxVelocidad; }
	FORCEINLINE UBoxComponent* GetHitComponent() { return HitComponent; }

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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Referencias")
	UBoxComponent* HitComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float DistanciaAtaque = 3.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Atributos")
	float StoppingDistance = 150.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Atributos")
	float MaxVelocidad = 600.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;
	
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY()
	float Velocidad = 200.f;
	
	UPROPERTY()
	UPecadorAnimInstance* PecadorAnimInstance;
	
	float ZOffset = 10.f; 
};
