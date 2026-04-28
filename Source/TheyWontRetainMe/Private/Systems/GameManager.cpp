#include "Systems/GameManager.h"
#include "LogMacros.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Characters/PlayerTemplate.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/ModoJuego.h"
#include "Widgets/ImpactInfo.h"

void UGameManager::ChangeScene()
{
}

void UGameManager::AddExp(float Quantity)
{
	LOG("Experiencia conseguida: %f", Quantity);
	CurrentEXP += Quantity;
	
	while (CurrentEXP >= NextLevelThreshold)
	{
		CurrentEXP -= NextLevelThreshold;
		CurrentLevel++; //Nuevo Nivel
		LevelsToUp++;
        
		NextLevelThreshold = BASE_XP * FMath::Pow(EXP_MULTIPLIER, CurrentLevel);
	}
	
	if (LevelsToUp > 0 && OnLevelUp.IsBound())
	{
		OnLevelUp.Broadcast(CurrentLevel,LevelsToUp);
		LevelsToUp = 0;
	}
}

void UGameManager::ShowImpacNumber(FVector ImpactPoint, AActor* InstigatorActor, float Damage)
{
	if (!ModoJuego) ModoJuego = Cast<AModoJuego>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!CurrentPlayer || !ModoJuego->GetImpactInfoClass()) return;
	APlayerController* PC = Cast<APlayerController>(CurrentPlayer->GetController());
	if (!PC) return;
	UImpactInfo* DamageWidget = CreateWidget<UImpactInfo>(GetWorld(), ModoJuego->GetImpactInfoClass());
	if (DamageWidget)
	{
		DamageWidget->SetNumberDamage(Damage);
		DamageWidget->SetDistanceFontSize(FVector::Distance(CurrentPlayer->GetActorLocation(),ImpactPoint));
		DamageWidget->AddToViewport();
		FVector2D ScreenPosition;
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
			PC, 
			InstigatorActor->GetActorLocation() + FVector(0,0,50), 
			ScreenPosition, 
			true))
		{
			DamageWidget->SetPositionInViewport(ScreenPosition);
		}
	}
}

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	NextLevelThreshold = BASE_XP;
}
