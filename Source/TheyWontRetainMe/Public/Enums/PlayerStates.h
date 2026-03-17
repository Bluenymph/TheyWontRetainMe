#pragma once

#include "CoreMinimal.h"
#include "PlayerStates.generated.h"

UENUM(BlueprintType)
enum class EPlayerInputMode : uint8
{
	EPIM_Free UMETA(DisplayName = "Movimiento Libre"),
	EPIM_Aiming UMETA(DisplayName = "Movimiento Apuntando")
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