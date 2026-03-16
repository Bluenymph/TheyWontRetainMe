#pragma once

#include "CoreMinimal.h"

#define LOG(Format, ...) \
UE_LOG(LogTemp, Warning, TEXT("%s"), \
	*FString::Printf(TEXT(Format), ##__VA_ARGS__))