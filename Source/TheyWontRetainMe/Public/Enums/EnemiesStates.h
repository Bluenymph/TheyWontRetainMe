#pragma once

#include "CoreMinimal.h"
#include "EnemiesStates.generated.h"

UENUM(BlueprintType)
enum class EEnemyBehaviourState : uint8
{
	EEBS_Idle UMETA(DisplayName = "Quieto"),
	EEBS_Emerging UMETA(DisplayName = "Emergiendo"),
	EEBS_Run UMETA(DisplayName = "Run"),
	EPWS_Injured UMETA(DisplayName = "Herido")
};
