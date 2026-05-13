#include "Characters/PlayerTemplate.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "LogMacros.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Actors/WeaponTemplate.h"
#include "Blueprint/UserWidget.h"
#include "Characters/EnemyTemplate.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterAttributes.h"
#include "HUD/MainHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/LevelUp.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Systems/AbilitiesManager.h"
#include "Systems/GameManager.h"

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

void APlayerTemplate::OnHitReceived_Implementation(float Damage, AActor* HitInstigator)
{
	IHiteableInterface::OnHitReceived_Implementation(Damage, HitInstigator);
	
	if (!CanTakeDmg) return;
	
	CanTakeDmg = false;
	CharacterAttributes->AttributesTakeDmg(Damage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_Invincible, this, &APlayerTemplate::OnInvincibleCD, TiempoInvencible, false);
	
	PlayerRecibirGolpe();
}

void APlayerTemplate::OnDamageInflicted(float Damage)
{
	if (CharacterAttributes && RoboVidaIndicador > 0.f)
	{
		CharacterAttributes->SetVidaActual(CharacterAttributes->GetVidaActual() + (Damage * RoboVidaIndicador));
	}
}

void APlayerTemplate::LevelUp(int32 CurrentLevel, int32 Levels)
{
	if (PlayerController && LevelUpWidget)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetPause(true);
		
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		
		LevelUpWidget->AddToViewport(10);
		LevelUpWidget->InitLevelUp();
	}
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
		
		OriginalBraking = GetCharacterMovement()->BrakingDecelerationWalking;
		OriginalFriction = GetCharacterMovement()->GroundFriction;
		
		PlayerController = PC;
	}
	
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerTemplate::OnHit);

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
	if (GetMesh())
	{
		DynamicMaterial_Mesh = GetMesh()->CreateDynamicMaterialInstance(0);
	}
	
	GameManager = GetGameInstance()->GetSubsystem<UGameManager>();
	GameManager->CurrentPlayer = this;
	
	AbilitiesManager = GetWorld()->GetSubsystem<UAbilitiesManager>();
	
	GameManager->OnLevelUp.AddUniqueDynamic(this, &APlayerTemplate::LevelUp);
	
	LevelUpWidget = CreateWidget<ULevelUp>(PlayerController, LevelUpWidgetClass);
	
}

void APlayerTemplate::MirarRaton(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookAxisVector.X * SensibilidadRaton * FACTOR_SENSIBILIDAD);
	AddControllerPitchInput(LookAxisVector.Y * SensibilidadRaton * -1.f * FACTOR_SENSIBILIDAD); 
}

void APlayerTemplate::MovimientoFrontal(const FInputActionValue& Value)
{
	if (!Controller || !CanChangeAnimation) return;
	float ParsedValue = Value.Get<float>();
	
	if (ParsedValue > 0.0f) PlayerMovementState = EPlayerMovementState::EPMS_WalkingForwards;
	else PlayerMovementState = EPlayerMovementState::EPMS_WalkingBackwards;
	
	AddMovementInput(Camera->GetForwardVector(), Value.Get<float>());
}

void APlayerTemplate::MovimientoVertical(const FInputActionValue& Value)
{
	if (!Controller || !CanChangeAnimation) return;
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
	if (!Controller || !CanChangeAnimation || !CanDodge || !AbilitiesManager->GetDodgeUnlocked()) return;
	CanChangeAnimation = false;
	CanDodge = false;
	PlayerMovementState = EPlayerMovementState::EPMS_Jumping;
	Esquivar();
}

void APlayerTemplate::Disparar()
{
	if (!Controller ||!CanChangeAnimation) return;
	if (ReloadEmote->IsPlaying()) return;
	if (CharacterAttributes->GetBalasActuales() <= 0) return;
	if (GetWorldTimerManager().IsTimerActive(TimerHandle_Disparo)) return;
	
	GetWorldTimerManager().SetTimer(TimerHandle_Disparo,this,&APlayerTemplate::OnTimerCdOut,CadenciaDisparo, false);
	
	OnFiringStateChanged.Broadcast(true);

	CharacterAttributes->SetBalasActuales(CharacterAttributes->GetBalasActuales() - 1);
	
	MainHUD->UpdateUIInfo(CharacterAttributes);
}

void APlayerTemplate::SoltarBoton()
{
	if (CanChangeAnimation) PlayerMovementState = EPlayerMovementState::EPMS_Idle;
}

void APlayerTemplate::PlayerRecibirGolpe()
{
	CanChangeAnimation = false;
	PlayerMovementState = EPlayerMovementState::EPMS_Hurt;
	Jump();
	DynamicMaterial_Mesh->SetScalarParameterValue("Invencible", 1.f);
}

void APlayerTemplate::PlayerDeath()
{
	GameManager->CurrentGameCycleMenu = 1;
	const FName LevelName = FName("MainMenu");
	UGameplayStatics::OpenLevel(this, LevelName, true);
}

void APlayerTemplate::UnPause()
{
	if (PlayerController && LevelUpWidget)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetPause(false);
		
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		
		LevelUpWidget->RemoveFromParent();
	}
}

void APlayerTemplate::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	if (!CanTakeDmg) return;
	
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			CanTakeDmg = false;
			CharacterAttributes->AttributesTakeDmg(20.f);
			
			if (CharacterAttributes->GetVidaActual() <= 0)
			{
				PlayerDeath();
				return;
			}
	
			GetWorldTimerManager().SetTimer(TimerHandle_Invincible, this, &APlayerTemplate::OnInvincibleCD, TiempoInvencible, false);

			PlayerRecibirGolpe();
			
		}
	}
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

void APlayerTemplate::Esquivar()
{
	FVector RollDirection = GetLastMovementInputVector();

	if (RollDirection.IsNearlyZero())
	{
		RollDirection = GetActorForwardVector();
	}

	FVector LaunchVelocity = RollDirection.GetSafeNormal() * VelocidadEsquive;
	LaunchVelocity.Z = 150.f; 
	
	FRotator TargetRotation = RollDirection.Rotation();
    
	TargetRotation.Yaw -= 90.f; 
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;

	GetMesh()->SetWorldRotation(TargetRotation);

	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	GetCharacterMovement()->GroundFriction = 0.5f;
	
	LaunchCharacter(LaunchVelocity, true, false);
	
	//Si tenemos la mejora de esquiva inmortal...
	if (AbilitiesManager->GetAbility_InmortalDodge())
	{
		CanTakeDmg = false;
	
		GetWorldTimerManager().SetTimer(TimerHandle_Invincible, this, &APlayerTemplate::OnInvincibleCD, 1, false);

		DynamicMaterial_Mesh->SetScalarParameterValue("Invencible", 1.f);
	}
	
	//Si tenemos la habilidad de esquiva audaz...
	if (AbilitiesManager->GetAbility_ReloadDodge())
	{
		OnReloadTimeOut();
	}
}

void APlayerTemplate::OnTimerCdOut()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Disparo);
}

void APlayerTemplate::OnInvincibleCD()
{
	CanTakeDmg = true;
	DynamicMaterial_Mesh->SetScalarParameterValue("Invencible", 0.f);
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

void APlayerTemplate::BeginDodgeTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandle_Esquivar,this,&APlayerTemplate::OnDodgeCD,CadenciaEsquiva,false);
}


void APlayerTemplate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MovimientoFrontal, ETriggerEvent::Triggered,this, &APlayerTemplate::MovimientoFrontal);
		EnhancedInputComponent->BindAction(IA_MovimientoFrontal, ETriggerEvent::Completed,this, &APlayerTemplate::SoltarBoton);
		
		EnhancedInputComponent->BindAction(IA_MovimientoVertical, ETriggerEvent::Triggered,this, &APlayerTemplate::MovimientoVertical);
		EnhancedInputComponent->BindAction(IA_MovimientoVertical, ETriggerEvent::Completed,this, &APlayerTemplate::SoltarBoton);
		
		EnhancedInputComponent->BindAction(IA_MirarRaton, ETriggerEvent::Triggered, this, &APlayerTemplate::MirarRaton);
		EnhancedInputComponent->BindAction(IA_Saltar, ETriggerEvent::Triggered, this, &APlayerTemplate::Saltar);
		EnhancedInputComponent->BindAction(IA_Disparar, ETriggerEvent::Triggered, this, &APlayerTemplate::Disparar);
		EnhancedInputComponent->BindAction(IA_Recargar, ETriggerEvent::Triggered, this, &APlayerTemplate::LlamarRecargar);
	}

}

