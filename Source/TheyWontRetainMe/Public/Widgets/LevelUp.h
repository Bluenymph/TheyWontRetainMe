#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUp.generated.h"

class UAbilityData;
class UAbilitiesManager;
class UButton;
class UTextBlock;
class UGameManager;

UCLASS()
class THEYWONTRETAINME_API ULevelUp : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void InitLevelUp();
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LevelsToUP;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUserWidget* WBP_SkillButton;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUserWidget* WBP_SkillButton_1;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUserWidget* WBP_SkillButton_2;
	
	UFUNCTION(BlueprintCallable)
	void OnSkillPressed();
	
private:
	UPROPERTY()
	UGameManager* GameManager;
	
	UPROPERTY()
	UAbilitiesManager* AbilitiesManager;
	
	UPROPERTY()
	TArray<UAbilityData*> ButtonsBindedAbilities;
	
	UFUNCTION()
	TArray<UAbilityData*> SelectThreeRngSkills(TArray<UAbilityData*> AllAbilitiesAvailable);
};
