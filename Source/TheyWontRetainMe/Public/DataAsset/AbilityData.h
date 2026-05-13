#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/BaseAbility.h"
#include "Engine/Texture2D.h"
#include "AbilityData.generated.h"

/*
 *Data asset para crear una habilidad. No olvides inluirlo en el DataBase del ModoJuego.
 */
UCLASS()
class THEYWONTRETAINME_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<UBaseAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	UTexture2D* Image;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FText Descripcion;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config") //10 muy facil, 1 muy dificil.
	int32 Probabilidad;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	int32 Limite; //Cantidad de veces que se puede conseguir la habilidad
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TArray<UAbilityData*> Requisitos;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bIsUnique;
};
