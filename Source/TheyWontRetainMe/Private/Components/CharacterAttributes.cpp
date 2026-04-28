#include "Components/CharacterAttributes.h"


UCharacterAttributes::UCharacterAttributes()
{
	PrimaryComponentTick.bCanEverTick = false;

}

const float UCharacterAttributes::AttributesTakeDmg(float Damage)
{
	VidaActual -= Damage;
	if (VidaActual <= 0.f)
	{
		VidaActual = 0.f;
		return -1.0f;
	}
	return VidaActual;
}

void UCharacterAttributes::Curar(float Cantidad)
{
	VidaActual += Cantidad;
	if (VidaActual > VidaMaxima) VidaActual = VidaMaxima;
}


void UCharacterAttributes::BeginPlay()
{
	Super::BeginPlay();

}

