#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributes.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEYWONTRETAINME_API UCharacterAttributes : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAttributes();

	const float AttributesTakeDmg(float Damage);
	
	UFUNCTION(BlueprintCallable)
	void Curar(float Cantidad);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float VidaMaxima = 60.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float VidaActual = 60.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	float Danio = 30.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	int BalasActuales = 15;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atributos")
	int MaxBalas = 15;

public:
	/** Getters */
	FORCEINLINE float GetVidaMaxima() const { return VidaMaxima; }
	FORCEINLINE float GetVidaActual() const { return VidaActual; }
	FORCEINLINE float GetDanio() const { return Danio; }
	FORCEINLINE int32 GetBalasActuales() const { return BalasActuales; }
	FORCEINLINE int32 GetMaxBalas() const { return MaxBalas; }

	/** Setters */
	FORCEINLINE void SetVidaMaxima(float NuevaMax) { VidaMaxima = NuevaMax; }
	FORCEINLINE void SetVidaActual(float NuevaVida) { VidaActual = FMath::Clamp(NuevaVida, 0.0f, VidaMaxima); }
	FORCEINLINE void SetDanio(float NuevoDanio) { Danio = NuevoDanio; }
	FORCEINLINE void SetBalasActuales(int32 NuevasBalas) { BalasActuales = FMath::Clamp(NuevasBalas, 0, MaxBalas); }
	FORCEINLINE void SetMaxBalas(int32 NuevoMax) { MaxBalas = NuevoMax; }

	FORCEINLINE void Recargar() { BalasActuales = MaxBalas; }
};
