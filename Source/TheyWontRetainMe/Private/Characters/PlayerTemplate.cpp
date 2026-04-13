#include "Characters/PlayerTemplate.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "LogMacros.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Actors/WeaponTemplate.h"
#include "Components/CharacterAttributes.h"
#include "HUD/MainHUD.h"

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

	CharacterAttributes = CreateDefaultSubobject<UCharacterAttributes>("Atributos");

	ReloadEmote = CreateDefaultSubobject<UPaperFlipbookComponent>("ReloadEmote");
	ReloadEmote->SetupAttachment(RootComponent);

	QuickReloadEmote = CreateDefaultSubobject<UPaperFlipbookComponent>("QuickReloadEmote");
	QuickReloadEmote->SetupAttachment(ReloadEmote);
	
	CurrentWeaponState = EPlayerWeaponSelected::EPWS_Pistol;
}

void APlayerTemplate::CambiarCadenciaDisparo(const float NuevaCadencia)
{
	if (NuevaCadencia < 0.1f) CadenciaDisparo = 0.1f;
	else CadenciaDisparo = NuevaCadencia;
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

	MainHUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!MainHUD) UE_LOG(LogTemp, Error, TEXT("El HUD no esta o no es el correcto. Debe ser BP_MainHUD."));
	
	SpawnWeapon();
	QuickReloadEmote->SetVisibility(false);
	QuickReloadEmote->SetLooping(false);

	ReloadEmote->SetVisibility(false);
	ReloadEmote->SetLooping(false);

	if (!ReloadEmote->OnFinishedPlaying.IsAlreadyBound(this, &APlayerTemplate::OnReloadTimeOut))
	{
		ReloadEmote->OnFinishedPlaying.AddDynamic(this, &APlayerTemplate::OnReloadTimeOut);
	}
}

void APlayerTemplate::MirarRaton(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookAxisVector.X * SensibilidadRaton * FACTOR_SENSIBILIDAD);
	AddControllerPitchInput(LookAxisVector.Y * SensibilidadRaton * -1.f * FACTOR_SENSIBILIDAD); 
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
	
	AddMovementInput( GetTransform().GetRotation().GetRightVector(), Value.Get<float>());
}

void APlayerTemplate::LlamarRecargar()
{
	if (!CharacterAttributes) return;

	//Recarga especial
	if (ReloadEmote->IsPlaying() && !CanQuickReload) return; 
	
	if (ReloadEmote->IsPlaying() && CanQuickReload)
	{
		CanQuickReload = false;
		bool IsQuickReload = ReloadEmote->GetPlaybackPosition() > ReloadThreshold && ReloadEmote->GetPlaybackPosition() < ReloadThreshold + QuickReloadSize;
		//LOG("¿Recarga exitosa?: %s", IsQuickReload ? TEXT("True") : TEXT("False"));
		if (IsQuickReload)
		{
			OnReloadTimeOut();
		}else
		{
			ReloadEmote->SetSpriteColor(FLinearColor::Red);
			QuickReloadEmote->SetSpriteColor(FLinearColor::Red);
		}
		return;
	}

	PlaceRngQuickReload();
	QuickReloadEmote->SetVisibility(true);
	ReloadEmote->PlayFromStart();
	ReloadEmote->SetVisibility(true);
}

void APlayerTemplate::Saltar()
{
	if (!Controller) return;
	
	PlayerMovementState = EPlayerMovementState::EPMS_Jumping;
	Jump();
}

void APlayerTemplate::Disparar()
{
	if (ReloadEmote->IsPlaying()) return;
	if (CharacterAttributes->GetBalasActuales() <= 0) return;
	if (GetWorldTimerManager().IsTimerActive(TimerHandle_Disparo)) return;
	
	GetWorldTimerManager().SetTimer(TimerHandle_Disparo,this,&APlayerTemplate::OnTimerCdOut,CadenciaDisparo, false);
	
	OnFiringStateChanged.Broadcast(true);

	CharacterAttributes->SetBalasActuales(CharacterAttributes->GetBalasActuales() - 1);
	
	MainHUD->UpdateUIInfo(CharacterAttributes);
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

void APlayerTemplate::OnTimerCdOut()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Disparo);
}

float APlayerTemplate::PlaceRngQuickReload()
{
	TArray ValoresPosibles = { -66.0f, 0.0f, 100.0f };
	float Resultado = ValoresPosibles[FMath::RandHelper(ValoresPosibles.Num())];
	FVector NuevaPosicion = QuickReloadEmote->GetRelativeLocation();

	NuevaPosicion.X = Resultado; 
	QuickReloadEmote->SetRelativeLocation(NuevaPosicion);

	if (Resultado == -66.0f)
	{
		ReloadThreshold = 0.3f;
	}else if (Resultado == 0.0f)
	{
		ReloadThreshold = 0.8f;
	}
	else
	{
		ReloadThreshold = 1.3f;
	}
	
	return Resultado;
}

void APlayerTemplate::OnReloadTimeOut()
{
	CharacterAttributes->Recargar();
	MainHUD->UpdateUIInfo(CharacterAttributes);

	CanQuickReload = true;

	QuickReloadEmote->SetVisibility(false);
	ReloadEmote->SetVisibility(false);
	ReloadEmote->SetSpriteColor(FLinearColor::White);
	QuickReloadEmote->SetSpriteColor(FLinearColor::White);
	ReloadEmote->Stop();
}

FRotator APlayerTemplate::GetPlayerCameraBoomYawRotation() const
{
	return FRotator(0,SpringArm->GetTargetRotation().Yaw,0);
}

TSubclassOf<ABulletTemplate> APlayerTemplate::GetPlayerBulletClass() const
{
	return BulletClass;
}

AWeaponTemplate* APlayerTemplate::GetPlayerCurrentWeapon()
{
	if (CurrentWeapon) return CurrentWeapon;
	return nullptr;
}

AWeaponTemplate* APlayerTemplate::GetPlayerCurrentSecondaryWeapon()
{
	if (CurrentSecondaryWeapon) return CurrentSecondaryWeapon;
	return nullptr;
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
		EnhancedInputComponent->BindAction(IA_Disparar, ETriggerEvent::Triggered, this, &APlayerTemplate::Disparar);
		EnhancedInputComponent->BindAction(IA_Recargar, ETriggerEvent::Triggered, this, &APlayerTemplate::LlamarRecargar);
	}

}

