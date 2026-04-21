#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityTemplate.generated.h"

UCLASS()
class THEYWONTRETAINME_API AAbilityTemplate : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbilityTemplate();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CodigoHabilidad;
	

};
