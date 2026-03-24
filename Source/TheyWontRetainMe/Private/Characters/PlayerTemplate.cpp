#include "Characters/PlayerTemplate.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "LogMacros.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Actors/WeaponTemplate.h"

APlayerTemplate::APlayerTemplate()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	SpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SpringArm->bUsePawnControlRotation = true;
	
	CurrentWeaponState = EPlayerWeaponSelected::EPWS_Pistol;
}

void APlayerTemplate::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (PlayerBaseInput)
			{
				Subsystem->AddMappingContext(PlayerBaseInput, 0);
			}
		}
		
		//Limitamos el angulo de la camara
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->ViewPitchMax = 20.0f; 
			PC->PlayerCameraManager->ViewPitchMin = -20.0f; 
		}
	}
	
	SpawnWeapon();
}

void APlayerTemplate::MirarRaton(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookAxisVector.X * SensibilidadRaton);
	AddControllerPitchInput(LookAxisVector.Y * SensibilidadRaton * -1.f); 
}

void APlayerTemplate::MovimientoFrontal(const FInputActionValue& Value)
{
	if (!Controller) return;
	float ParsedValue = Value.Get<float>();
	
	if (ParsedValue > 0.0f) PlayerMovementState = EPlayerMovementState::EPMS_WalkingForwards;
	else PlayerMovementState = EPlayerMovementState::EPMS_WalkingBackwards;
	
	AddMovementInput(Camera->GetForwardVector(), Value.Get<float>());
}

void APlayerTemplate::MovimientoVertical(const FInputActionValue& Value)
{
	if (!Controller) return;
	float ParsedValue = Value.Get<float>();
	
	if (ParsedValue > 0.0f) PlayerMovementState = EPlayerMovementState::EPMS_StrafeRight;
	else PlayerMovementState = EPlayerMovementState::EPMS_StrafeLeft;
	
	AddMovementInput(Camera->GetRightVector(), Value.Get<float>());
}

void APlayerTemplate::Saltar()
{
	if (!Controller) return;
	
	PlayerMovementState = EPlayerMovementState::EPMS_Jumping;
	Jump();
}

void APlayerTemplate::SpawnWeapon()
{
	if (CurrentWeaponState == EPlayerWeaponSelected::EPWS_Pistol )
	{
		if (WeaponClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			CurrentWeapon = GetWorld()->SpawnActor<AWeaponTemplate>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			CurrentSecondaryWeapon = GetWorld()->SpawnActor<AWeaponTemplate>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			
			if (CurrentWeapon && CurrentSecondaryWeapon)
			{
				CurrentWeapon->AttachToComponent(
					GetMesh(), 
					FAttachmentTransformRules::SnapToTargetIncludingScale, 
					FName("LeftPistolSocket")
				);
				
				CurrentSecondaryWeapon->AttachToComponent(
					GetMesh(), 
					FAttachmentTransformRules::SnapToTargetIncludingScale, 
					FName("RightPistolSocket")
				);
			}
		}
	}
}


void APlayerTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerMovementState = EPlayerMovementState::EPMS_Idle;
}

void APlayerTemplate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MovimientoFrontal, ETriggerEvent::Triggered,this, &APlayerTemplate::MovimientoFrontal);
		EnhancedInputComponent->BindAction(IA_MovimientoVertical, ETriggerEvent::Triggered,this, &APlayerTemplate::MovimientoVertical);
		EnhancedInputComponent->BindAction(IA_MirarRaton, ETriggerEvent::Triggered, this, &APlayerTemplate::MirarRaton);
		EnhancedInputComponent->BindAction(IA_Saltar, ETriggerEvent::Triggered, this, &APlayerTemplate::Saltar);
	}

}

