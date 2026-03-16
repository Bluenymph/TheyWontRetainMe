#include "Characters/PlayerTemplate.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "LogMacros.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerTemplate::APlayerTemplate()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	SpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
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
	}
	
}

void APlayerTemplate::MirarRaton(const FInputActionValue& Value)
{
	if (!SpringArm) return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	
	float NewYaw = CurrentRotation.Yaw + (LookAxisVector.X * SensibilidadRaton);
	float NewPitch = CurrentRotation.Pitch + (LookAxisVector.Y * SensibilidadRaton);
	
	NewPitch = FMath::Clamp(NewPitch, -20.0f, 10.0f); 
	
	SpringArm->SetRelativeRotation(FRotator(NewPitch, NewYaw, 0.f));
}

void APlayerTemplate::MovimientoFrontal(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	AddMovementInput(Camera->GetForwardVector(), Value.Get<float>());
}

void APlayerTemplate::MovimientoVertical(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	AddMovementInput(Camera->GetRightVector(), Value.Get<float>());
}

void APlayerTemplate::Saltar()
{
	if (!Controller) return;
	
	Jump();
}


void APlayerTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

