#include "Characters/PlayerTemplate.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "LogMacros.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerTemplate::APlayerTemplate()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PlayerInputMode = EPlayerInputMode::EPIM_Free;

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
	float ParsedValue = Value.Get<float>();
	
	if (ParsedValue > 0.0f) PlayerMovementState = EPlayerMovementState::EPMS_WalkingForwards;
	else PlayerMovementState = EPlayerMovementState::EPMS_WalkingBackwards;
	
	AddMovementInput(Camera->GetForwardVector(), Value.Get<float>());
	
	if (PlayerInputMode == EPlayerInputMode::EPIM_Aiming) LookForward();
	else LookToDirection();
}

void APlayerTemplate::MovimientoVertical(const FInputActionValue& Value)
{
	if (!Controller) return;
	float ParsedValue = Value.Get<float>();
	
	if (ParsedValue > 0.0f) PlayerMovementState = EPlayerMovementState::EPMS_StrafeRight;
	else PlayerMovementState = EPlayerMovementState::EPMS_StrafeLeft;
	
	AddMovementInput(Camera->GetRightVector(), Value.Get<float>());
	
	if (PlayerInputMode == EPlayerInputMode::EPIM_Aiming) LookForward();
	else LookToDirection();
}

void APlayerTemplate::Apuntar()
{
	LOG("Apuntando")
	PlayerInputMode = EPlayerInputMode::EPIM_Aiming;
}

void APlayerTemplate::DejarApuntar()
{
	LOG("Ya no apuntamos")
	PlayerInputMode = EPlayerInputMode::EPIM_Free;
}

void APlayerTemplate::Saltar()
{
	if (!Controller) return;
	
	PlayerMovementState = EPlayerMovementState::EPMS_Jumping;
	Jump();
}

void APlayerTemplate::LookForward()
{
	FRotator SpringArmRotation = SpringArm->GetComponentRotation();
	FRotator NewRotation(0.f,SpringArmRotation.Yaw, 0.f);
	NewRotation.Yaw -= 90.f;
	
	//Smooth transition
	float RotationSpeed = 10.f;
	FRotator SmoothRotation = FMath::RInterpTo(
		GetMesh()->GetRelativeRotation(),
		NewRotation,
		GetWorld()->GetDeltaSeconds(),
		RotationSpeed
	);

	GetMesh()->SetRelativeRotation(SmoothRotation);
}

void APlayerTemplate::LookToDirection()
{
	FVector InputVector = GetCharacterMovement()->GetLastInputVector();
	FRotator InputRotation = InputVector.ToOrientationRotator();
	FRotator NewRotation(0.f,InputRotation.Yaw, 0.f);
	NewRotation.Yaw -= 90.f;
	
	//Smooth transition
	float RotationSpeed = 10.f;
	FRotator SmoothRotation = FMath::RInterpTo(
		GetMesh()->GetRelativeRotation(),
		NewRotation,
		GetWorld()->GetDeltaSeconds(),
		RotationSpeed
	);

	GetMesh()->SetRelativeRotation(SmoothRotation);
}


void APlayerTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
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
		EnhancedInputComponent->BindAction(IA_Apuntar, ETriggerEvent::Started, this, &APlayerTemplate::Apuntar);
		EnhancedInputComponent->BindAction(IA_Apuntar, ETriggerEvent::Completed, this, &APlayerTemplate::DejarApuntar);
	}

}

