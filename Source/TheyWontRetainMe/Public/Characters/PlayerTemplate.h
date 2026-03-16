#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerTemplate.generated.h"

UCLASS()
class THEYWONTRETAINME_API APlayerTemplate : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerTemplate();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	

};
