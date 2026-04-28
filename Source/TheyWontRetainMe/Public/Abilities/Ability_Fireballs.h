#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_Fireballs.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THEYWONTRETAINME_API UAbility_Fireballs : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<AActor> VisualActorClass;

	virtual void ActivateAbility(AActor* InOwner) override;
	virtual void TickAbility(float DeltaTime) override;
	virtual void OnVisualOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BiggerRadius();
	
	UFUNCTION()
	FORCEINLINE void SetDamage(const float NewDmg) { Damage = NewDmg; }
	
	UFUNCTION()
	FORCEINLINE float GetDamage() { return Damage; }
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.f;

};
