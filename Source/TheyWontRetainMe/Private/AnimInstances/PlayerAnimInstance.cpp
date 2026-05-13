#include "AnimInstances/PlayerAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "LogMacros.h"
#include "Actors/BulletTemplate.h"
#include "Actors/WeaponTemplate.h"
#include "Characters/PlayerTemplate.h"
#include "Systems/BulletPoolSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/AbilitiesManager.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (PlayerCharacter)
	{
		CurrentMovementState = PlayerCharacter->GetPlayerMovementState();
		CurrentActionState = PlayerCharacter->GetPlayerActionState();
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<APlayerTemplate>(TryGetPawnOwner());
		if (PlayerCharacter)
		{
			PlayerCharacter->OnFiringStateChanged.AddDynamic(this, &UPlayerAnimInstance::ActualizarEstadoDisparo);
		}
	}
	
	if (!AbilitiesManager && GetWorld())
	{
		AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	}
}

void UPlayerAnimInstance::ActualizarEstadoDisparo(bool bNuevoEstado)
{
	if (!PlayerCharacter) return;
	
	if (ManoDisparo == 0 && IzquierdaFuegoMontage)
	{
		Montage_Play(IzquierdaFuegoMontage);
		
		
	}else if (ManoDisparo == 1 && DerechaFuegoMontage)
	{
		Montage_Play(DerechaFuegoMontage);
	}
	
	if (ManoDisparo == 1) ManoDisparo = 0;
	else ManoDisparo = 1;
	
	Disparar();
}

void UPlayerAnimInstance::AnimNotify_FinDeAnimacion()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->GetPlayerMovementState() = EPlayerMovementState::EPMS_Idle;
		PlayerCharacter->GetCanChangeAnimation() = true;
		
		PlayerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = PlayerCharacter->GetOriginalBraking();
		PlayerCharacter->GetCharacterMovement()->GroundFriction = PlayerCharacter->GetOriginalFriction();
		PlayerCharacter->GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
}

void UPlayerAnimInstance::AnimNotify_FinDeEsquive()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->BeginDodgeTimer();
	}
}

void UPlayerAnimInstance::Disparar()
{
	if (!PlayerCharacter) return;
	
	//LOG("DISPARANDO DESDE ANIMATOR %i", ManoDisparo);
	
	AWeaponTemplate* ArmaDisparo = PlayerCharacter->GetPlayerCurrentSecondaryWeapon();	
	if (ManoDisparo == 1) ArmaDisparo = PlayerCharacter->GetPlayerCurrentWeapon();
	
	//DISPARAAAAR
	UBulletPoolSubsystem* BulletSubsystem = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();

	if (BulletSubsystem)
	{
		FRotator DesiredRotation = PlayerCharacter->GetPlayerCameraBoomYawRotation();
		FVector DesiredPosition = ArmaDisparo->GetActorLocation();
		
		ABulletTemplate* OriginalBullet = BulletSubsystem->GetBulletFromPool(
			PlayerCharacter->GetPlayerBulletClass(), 
			DesiredPosition, 
			DesiredRotation, 
			PlayerCharacter->GetCurrentBulletDamage(), 
			PlayerCharacter);
		
		float SideMultiplier = 1.0f;

		for (int32 i = 0; i < AbilitiesManager->GetExtraBulletsQuantity(); i++)
		{
			SideMultiplier *= -1.0f;
    
			float Offset = 100.0f * (i / 2 + 1);
			FVector NewPosition = DesiredPosition;
			NewPosition.Y += Offset * SideMultiplier;

			ABulletTemplate* Bullet = BulletSubsystem->GetBulletFromPool(
				PlayerCharacter->GetPlayerBulletClass(), 
				NewPosition, 
				DesiredRotation, 
				PlayerCharacter->GetCurrentBulletDamage() / 3, 
				PlayerCharacter);
			
			if(Bullet) Bullet->ReduceBulletSize(1.5f);

		}
		
	}
}
