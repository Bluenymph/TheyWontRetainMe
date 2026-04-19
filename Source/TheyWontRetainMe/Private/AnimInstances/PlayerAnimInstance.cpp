#include "AnimInstances/PlayerAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "LogMacros.h"
#include "Actors/WeaponTemplate.h"
#include "Characters/PlayerTemplate.h"
#include "Systems/BulletPoolSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		LOG("HEHE")
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
		BulletSubsystem->GetBulletFromPool(PlayerCharacter->GetPlayerBulletClass(), 
			ArmaDisparo->GetActorLocation(), DesiredRotation, PlayerCharacter->GetCurrentBulletDamage());
	}
}
