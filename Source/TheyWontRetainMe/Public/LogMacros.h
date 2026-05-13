#pragma once

#include "CoreMinimal.h"

#define LOG(Format, ...) \
UE_LOG(LogTemp, Warning, TEXT("[%s : %d] %s"), \
	*FString(__FUNCTION__), __LINE__, \
	*FString::Printf(TEXT(Format), ##__VA_ARGS__))

#define DEBUG(ID, Color, Texto, ...) \
if (GEngine) GEngine->AddOnScreenDebugMessage(ID, 5.f, FColor::Color, FString::Printf(TEXT(Texto), ##__VA_ARGS__))