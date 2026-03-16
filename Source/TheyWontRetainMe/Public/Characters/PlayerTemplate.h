#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerTemplate.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ajustes")
	float SensibilidadRaton = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputMappingContext* PlayerBaseInput;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_MovimientoFrontal;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_MovimientoVertical;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_MirarRaton;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inputs")
	UInputAction* IA_Saltar;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;
	
	UFUNCTION(BlueprintCallable)
	void MirarRaton(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void MovimientoFrontal(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void MovimientoVertical(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void Saltar();

};
