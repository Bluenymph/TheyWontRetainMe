#pragma once

#include "CoreMinimal.h"
#include "EnemiesStates.generated.h"

UENUM(BlueprintType)
enum class EEnemyBehaviourState : uint8
{
	EEBS_Idle UMETA(DisplayName = "Normal"),
	EPWS_Injured UMETA(DisplayName = "Herido")
};
