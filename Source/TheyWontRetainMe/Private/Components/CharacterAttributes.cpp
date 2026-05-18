#include "Components/CharacterAttributes.h"
#include "Systems/GameManager.h"

UCharacterAttributes::UCharacterAttributes()
{
	PrimaryComponentTick.bCanEverTick = false;

}

const float UCharacterAttributes::AttributesTakeDmg(float Damage)
{
	float Resultado;
	VidaActual -= Damage;
	if (VidaActual <= 0.f)
	{
		Resultado = -1.0f;
	}else Resultado = VidaActual;
	
	if (GetOwner()->ActorHasTag("Player"))
	{
		float PlayerDmgd = FCString::Atof(*GameManager->GameStatistics.PlayerDmgReceived);
		PlayerDmgd += Damage;
		GameManager->GameStatistics.PlayerDmgReceived = FString::SanitizeFloat(PlayerDmgd);
	}
	else if (GetOwner()->ActorHasTag("Enemy"))
	{
		float PlayerDmg = FCString::Atof(*GameManager->GameStatistics.PlayerDmgInflinged);
		PlayerDmg += Damage;
		GameManager->GameStatistics.PlayerDmgInflinged = FString::Printf(TEXT("%.2f"), PlayerDmg);
	}
	
	return Resultado;
}

void UCharacterAttributes::Curar(float Cantidad)
{
	VidaActual += Cantidad;
	if (VidaActual > VidaMaxima) VidaActual = VidaMaxima;
	
	if (GetOwner()->ActorHasTag("Player"))
	{
		float PlayerHeal = FCString::Atof(*GameManager->GameStatistics.PlayerAutoHeal);
		PlayerHeal += Cantidad;
		GameManager->GameStatistics.PlayerDmgReceived = FString::SanitizeFloat(PlayerHeal);
	}
}


void UCharacterAttributes::BeginPlay()
{
	Super::BeginPlay();
	
	GameManager = GetOwner()->GetGameInstance()->GetSubsystem<UGameManager>();

}

