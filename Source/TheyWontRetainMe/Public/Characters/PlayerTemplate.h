#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/PlayerStates.h"
#include "Interfaces/HiteableInterface.h"
#include "PlayerTemplate.generated.h"

class AMainHUD;
class UUserInterface;
class ABulletTemplate;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCharacterAttributes;
class AWeaponTemplate;
class UPaperFlipbookComponent;
class UMaterialInstanceDynamic;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFiringStateChanged, bool, bIsFiring);

UCLASS()
class THEYWONTRETAINME_API APlayerTemplate : public ACharacter, public IHiteableInterface
{
	GENERATED_BODY()

public:
	APlayerTemplate();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnFiringStateChanged OnFiringStateChanged;
	
	void CambiarCadenciaDisparo(const float NuevaCadencia);
	
	void OnHitReceived_Implementation(float Damage) override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float SensibilidadRaton = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	UCharacterAttributes* CharacterAttributes;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float CadenciaEsquiva = 10.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float CadenciaDisparo = 0.5f;
	
	//El daño que hace la bala
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float PotenciaDisparo = 20.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float VelocidadEsquive = 1200.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float TiempoInvencible = 2.5f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	TSubclassOf<AWeaponTemplate> WeaponClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	TSubclassOf<ABulletTemplate> BulletClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	AMainHUD* MainHUD;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	UPaperFlipbookComponent* ReloadEmote;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	UPaperFlipbookComponent* QuickReloadEmote;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputMappingContext* PlayerBaseInput;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_MovimientoFrontal;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_MovimientoVertical;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_MirarRaton;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_Saltar;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_Disparar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_Recargar;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;
	
	UFUNCTION(BlueprintCallable)
	void MirarRaton(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void MovimientoFrontal(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void MovimientoVertical(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void LlamarRecargar();
	
	UFUNCTION(BlueprintCallable)
	void Saltar();
	
	UFUNCTION(BlueprintCallable)
	void Disparar();
	
	UFUNCTION(BlueprintCallable)
	void SoltarBoton();
	
	UFUNCTION(BlueprintCallable)
	void PlayerRecibirGolpe();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	const float MAX_SHOOT_CD = 0.1f;
	const float FACTOR_SENSIBILIDAD = 0.1f;
	
	FTimerHandle TimerHandle_Disparo;
	FTimerHandle TimerHandle_Esquivar;
	FTimerHandle TimerHandle_Invincible;
	
	UFUNCTION()
	void SpawnWeapon();
	
	UFUNCTION()
	void Esquivar();
	
	UPROPERTY()
	EPlayerMovementState PlayerMovementState;
	
	UPROPERTY()
	EPlayerActionState PlayerActionState;
	
	UPROPERTY()
	EPlayerWeaponSelected CurrentWeaponState;
	
	UPROPERTY()
	AWeaponTemplate* CurrentWeapon;
	
	UPROPERTY()
	AWeaponTemplate* CurrentSecondaryWeapon; //Para la pistola secundaria

	UPROPERTY()
	bool CanQuickReload = true;
	
	UPROPERTY()
	bool CanChangeAnimation = true;
	
	UPROPERTY()
	bool CanDodge = true;
	
	UPROPERTY()
	bool CanTakeDmg = true;
	
	UPROPERTY()
	float QuickReloadSize = 0.6f;
	
	UPROPERTY()
	float ReloadThreshold = 0.3f;
	
	UPROPERTY()
	float OriginalBraking;
	
	UPROPERTY()
	float OriginalFriction;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_Mesh;
	
	UFUNCTION()
	void OnTimerCdOut();
	
	UFUNCTION()
	FORCEINLINE void OnDodgeCD() { CanDodge = true;}

	UFUNCTION()
	void OnInvincibleCD();
	
	UFUNCTION()
	float PlaceRngQuickReload();

	UFUNCTION()
	void OnReloadTimeOut();
	
public:
	FRotator GetPlayerCameraBoomYawRotation() const;
	TSubclassOf<ABulletTemplate> GetPlayerBulletClass() const;
	AWeaponTemplate* GetPlayerCurrentWeapon();
	AWeaponTemplate* GetPlayerCurrentSecondaryWeapon();
	
	UFUNCTION(BlueprintCallable)
	void BeginDodgeTimer();
	
	FORCEINLINE EPlayerMovementState &GetPlayerMovementState() { return PlayerMovementState; }
	FORCEINLINE EPlayerActionState GetPlayerActionState() const { return PlayerActionState; }
	FORCEINLINE float GetCurrentBulletDamage() const { return PotenciaDisparo;}
	FORCEINLINE bool &GetCanChangeAnimation() { return CanChangeAnimation; }
	FORCEINLINE float GetOriginalBraking() const { return OriginalBraking; }
	FORCEINLINE float GetOriginalFriction() const { return OriginalFriction; }
	FORCEINLINE UMaterialInstanceDynamic* GetDynamicMaterial_Mesh() const { return DynamicMaterial_Mesh; }
	

};
