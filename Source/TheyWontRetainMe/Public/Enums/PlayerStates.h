#pragma once

#include "CoreMinimal.h"
#include "PlayerStates.generated.h"

UENUM(BlueprintType)
enum class EPlayerWeaponSelected : uint8
{
	EPWS_Pistol UMETA(DisplayName = "Dobles pistolas"),
	EPWS_Shotgun UMETA(DisplayName = "Escopetas")
};


UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	EPMS_Idle UMETA(DisplayName = "Idle"),
	EPMS_WalkingForwards UMETA(DisplayName = "Andando hacia delante"),
	EPMS_WalkingBackwards UMETA(DisplayName = "Andando hacia atras"),
	EPMS_StrafeRight UMETA(DisplayName = "Andando hacia derecha"),
	EPMS_StrafeLeft UMETA(DisplayName = "Andando hacia izquierda"),
	EPMS_Jumping UMETA(DisplayName = "Saltando")
};

UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	EPAS_Idle UMETA(DisplayName = "Idle"),
	EPAS_Shooting UMETA(DisplayName = "Disparando"),
	EPAS_Reloading UMETA(DisplayName = "Recargando")
};