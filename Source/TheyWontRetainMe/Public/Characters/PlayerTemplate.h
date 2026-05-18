#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/PlayerStates.h"
#include "Interfaces/HiteableInterface.h"
#include "PlayerTemplate.generated.h"

class UAbilitiesManager;
class ULevelUp;
class AMainHUD;
class UMenuPausa;
class USoundBase;
class USoundWave;
class UUserWidget;
class UAbilityData;
class UInputAction;
class UGameManager;
class UUserInterface;
class ABulletTemplate;
class AWeaponTemplate;
class UCameraComponent;
class APlayerController;
class USpringArmComponent;
class UInputMappingContext;
class UCharacterAttributes;
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
	
	void OnHitReceived_Implementation(float Damage, AActor* HitInstigator) override;
	
	UFUNCTION(BlueprintCallable)
	void OnDamageInflicted(float Damage);
	
	UFUNCTION(BlueprintCallable)
	void LevelUp(int32 CurrentLevel, int32 Levels);
	
	UFUNCTION(BlueprintCallable)
	void UnPause();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float SensibilidadRaton = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	UCharacterAttributes* CharacterAttributes;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float CadenciaEsquiva = 5.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float CadenciaDisparo = 0.5f;
	
	//El daño que hace la bala
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float PotenciaDisparo = 20.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float VelocidadEsquive = 1200.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float TiempoInvencible = 2.5f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float RoboVidaIndicador = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Propiedades")
	float ProbabilidadCritico = 0.1f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	TSubclassOf<AWeaponTemplate> WeaponClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	TSubclassOf<ABulletTemplate> BulletClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Referencias")
	USoundBase* MetaSoundPlantilla;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Referencias")
	USoundWave* SonidoDisparo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Referencias")
	USoundWave* SonidoRecarga;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	AMainHUD* MainHUD;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	UPaperFlipbookComponent* ReloadEmote;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	UPaperFlipbookComponent* QuickReloadEmote;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	TSubclassOf<UUserWidget> LevelUpWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Referencias")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	
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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_Pausar;
	
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
	
	UFUNCTION(BlueprintCallable)
	void PlayerDeath();
	
	UFUNCTION(BlueprintCallable)
	void Pausar();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	const float MAX_SHOOT_CD = 0.1f;
	const float FACTOR_SENSIBILIDAD = 0.1f;
	
	FTimerHandle TimerHandle_Disparo;
	FTimerHandle TimerHandle_Esquivar;
	FTimerHandle TimerHandle_Invincible;
	FTimerHandle TimerHandle_SecondsAlive;
	
	UPROPERTY()
	int32 SecondAlive = 0;
	
	UPROPERTY()
	UGameManager* GameManager;
	
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
	APlayerController* PlayerController;
	
	UPROPERTY()
	ULevelUp* LevelUpWidget;
	
	UPROPERTY()
	UMenuPausa* MenuPausa;
	
	UPROPERTY()
	UAbilitiesManager* AbilitiesManager;
	
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
	bool bIsOnLevelUpMenu = false;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_Mesh;
	
	UFUNCTION()
	void OnAddSecondsAlive();
	
	UFUNCTION()
	void OnTimerCdOut();
	
	UFUNCTION()
	void OnDodgeCD();

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
	FORCEINLINE float GetCadenciaDisparo() const { return CadenciaDisparo; }
	FORCEINLINE float GetRoboVidaIndicador() const { return RoboVidaIndicador; }
	FORCEINLINE UCharacterAttributes* GetCharacterAttributes() { return CharacterAttributes; }
	FORCEINLINE float GetPotenciaDisparo() const { return PotenciaDisparo; }
	FORCEINLINE float GetProbabilidadCritico() const { return ProbabilidadCritico; }
	FORCEINLINE int32 GetSecondsAlive() const { return SecondAlive; }
	
	FORCEINLINE void SetCadenciaDisparo(const float NewCadency) { CadenciaDisparo = NewCadency; }
	FORCEINLINE void SetRoboVidaIndicador(const float NuevoIndicador) { RoboVidaIndicador = NuevoIndicador; }
	FORCEINLINE void SetPotenciaDisparo(const float NuevaPotencia) { PotenciaDisparo = NuevaPotencia; }
	FORCEINLINE void SetProbabilidadCritico(const float NewCrit) { ProbabilidadCritico = NewCrit; }
	FORCEINLINE void SetbIsOnLevelUpMenu(const bool NewValue) { bIsOnLevelUpMenu = NewValue; }
	

};
