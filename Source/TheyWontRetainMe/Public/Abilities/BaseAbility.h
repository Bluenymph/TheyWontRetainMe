#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseAbility.generated.h"

UCLASS(BlueprintType, Blueprintable)
class THEYWONTRETAINME_API UBaseAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Ability")
	FText AbilityName;
	
	UPROPERTY(BlueprintReadWrite, Category = "Ability")
	AActor* VisualActor;

	UPROPERTY(BlueprintReadWrite, Category = "Ability")
	AActor* AbilityOwner;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void ActivateAbility(AActor* InOwner);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void TickAbility(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void OnVisualOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
